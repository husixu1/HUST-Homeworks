#include "httpserver.h"
#include "globalconfig.h"
#include "dbresponse.h"

#include <QJsonDocument>
#include <QRandomGenerator>

using namespace qhttp::server;

QSet<DBConnector *> HttpServer::dbConnectors = QSet<DBConnector *>();

HttpServer::HttpServer() : QHttpServer(nullptr) {

}

bool HttpServer::startServer(quint16 port) {
    bool res = listen(QHostAddress::Any, port, requestHandler);
    if (res)
        qInfo("http server started");
    else
        qFatal("failed to start http server");
    return res;
}

void HttpServer::addDBConnector(DBConnector *connector) {
    if (connector)
        dbConnectors.insert(connector);
}

void HttpServer::removeDBConnector(DBConnector *connector) {
    if (connector)
        dbConnectors.remove(connector);
}

void HttpServer::respondToRequest(QHttpResponse *res, DBResponse data) {
    // invoke method
    QMetaObject::invokeMethod(this, "respond", Qt::QueuedConnection,
            Q_ARG(qhttp::server::QHttpResponse *, res),
            Q_ARG(QByteArray, QJsonDocument(data.toJson()).toJson()) );
}

void HttpServer::requestHandler(QHttpRequest *req, QHttpResponse *res) {
    qInfo("request received");
    QByteArray *data = new QByteArray();

    // collect all datas
    req->onData([ = ](QByteArray partData) {
        *data += partData;
    });

    // after all data is collected
    req->onEnd([ = ]() {
        // convert QHttpRequest to protocol-compatible HttpRequest
        DBRequest request = DBRequest::fromJson(
                        QJsonDocument::fromJson(*data).object());
        delete data;

        // if there's no connector
        if (!dbConnectors.size()) {
            qWarning("no DBConnector online (no request processor)");
            // response internal error
            DBResponse response;
            response.setCommonError(DBResponse::CommonErrorCode::internalError);
            res->end(QJsonDocument(response.toJson()).toJson());
            return;
        }

        // a random generator
        static QRandomGenerator random;
        qint32 startPoint = (static_cast<qint32>(random.generate()) %
                        dbConnectors.size() );

        // send to random connector
        bool result = (*(dbConnectors.begin() + startPoint))
                ->addToRequestQueue(request, res);

        // if the queue is full, try to add to other queues
        if (!result) {
            for (int i = 1; i < dbConnectors.size(); ++i) {
                result = (*(dbConnectors.begin() +
                                        (startPoint + i) % dbConnectors.size()))
                        ->addToRequestQueue(request, res);
                if (result)
                    break;
            }
        }

        // if failed to add to any queue, response
        if (!result) {
            qWarning("too busy to process any more request");

            // response server busy
            DBResponse response;
            response.setCommonError(DBResponse::CommonErrorCode::serverBusy);
            res->end(QJsonDocument(response.toJson()).toJson());
        }
    });
}

void HttpServer::respond(QHttpResponse *res, QByteArray data) {
    res->end(data);
}
