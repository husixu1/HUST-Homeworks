#include "httpclient.h"
#include "globalconfig.h"

#include <QJsonDocument>
#include <QDebug>

using namespace qhttp::client;

HttpClient::HttpClient() : QHttpClient(nullptr) {
    maxTokenNum = 0;
    isBusy = false;
}

HttpClient &HttpClient::getInsatance() {
    // TODO: make app it's parent
    static HttpClient instance;
    return instance;
}

HttpClient::Token HttpClient::sendRequest(DBRequest request) {
    Token result;

    // return token to the pool and relase response
    // when token is abandoned
    auto tokenDeleter = [&](quint64 * a) {
        // return token to token pool
        if (*a == maxTokenNum)
            --maxTokenNum;
        else
            tokenPool.push_back(*a);
        // remove response (if has one)
        responseMap.remove(*a);
        // remove token itself
        qDebug() << "token " << *a << " deleted, now max token num is " << maxTokenNum;
        delete a;
    };

    // release one token from the pool
    if (tokenPool.empty()) {
        ++maxTokenNum;
        result = QSharedPointer<quint64>(new quint64(maxTokenNum), tokenDeleter);
    } else {
        result = QSharedPointer<quint64>(new quint64(tokenPool.front()), tokenDeleter);
        tokenPool.pop_front();
    }
    qDebug() << "token " << *result << " created, now max token num is " << maxTokenNum;

    // execute sendRequest in httpclient thread
    QMetaObject::invokeMethod(this, STRINGIFY(_sendRequest),
            Qt::QueuedConnection,
            Q_ARG(DBRequest, request),
            Q_ARG(quint64, *result));

    // return the token for later use
    return result;
}

DBResponse HttpClient::getResponse(HttpClient::Token token) {
    if (responseMap.contains(*token))
        return responseMap.value(*token);
    throw std::logic_error("response dose not exists");
}

void HttpClient::_sendRequest(DBRequest _request, quint64 token) {
    using namespace qhttp::client;

    if (isBusy) {
        busyCache.push_back({_request, token});
        return;
    }
    isBusy = true;

    // post the request to the server
    request(
            qhttp::EHTTP_POST, Config::serverUrl,
    [_request](QHttpRequest * httpReqest) {
        // send the request
        QByteArray message = QJsonDocument(_request.toJson()).toJson();
        qDebug().noquote() << message;

        // **IMPORTANT** content-length MUST be set or the server won't
        // receive the body
        httpReqest->addHeaderValue("content-length", message.size());
        httpReqest->end(message);
    },
    [token, this](QHttpResponse * response) {
        // insert to responseMap when all response is retrieved
        response->onEnd([this, response, token]() {
            qInfo() << "response " << token << " retrived form server";
            DBResponse dbResponse = DBResponse::formJson(
                            QJsonDocument::fromJson(
                                    response->collectedData()).object());
            responseMap.insert(token, dbResponse);
            qDebug().noquote() << response->collectedData();
            qDebug() << "Common error:" << dbResponse.getCommonError();
            qDebug() << "Service error:" << dbResponse.getServiceError();
            emit serverResponsed(token);

            isBusy = false;
            // process remaining request
            if (!busyCache.empty()) {
                QMetaObject::invokeMethod(this, STRINGIFY(_sendRequest),
                        Qt::QueuedConnection,
                        Q_ARG(DBRequest, busyCache.head().first),
                        Q_ARG(quint64, busyCache.head().second));
                busyCache.pop_front();
            }
        });
        //try to collect response
        response->collectData();
    });
}
