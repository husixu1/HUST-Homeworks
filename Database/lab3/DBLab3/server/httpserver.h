#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "qhttpserver.hpp"
#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"
#include "dbconnector.h"

#include <QObject>
#include <QDebug>
#include <QSet>
#include <QMap>

class HttpServer : public qhttp::server::QHttpServer {
    Q_OBJECT
private:
    /// @brief all database connecotrs;
    static QSet<DBConnector *> dbConnectors;

public:
    /// @brief constructor
    HttpServer();

    /// @brief try (re)start the http server
    /// @param[in] port the port to use
    /// @return if the server is started
    bool startServer(quint16 port);

    // TODO: add stopServer

    /// @brief register an database connector;
    /// @param[in] connector the connector to add
    void addDBConnector(DBConnector *connector);

    /// @brief remove an database connector;
    /// @param[in] connector the connector to remove
    void removeDBConnector(DBConnector *connector);

    /// @brief response to an request in the http server thread
    /// @threadsafe
    void respondToRequest(qhttp::server::QHttpResponse *res, DBResponse data);

private:
    /// @brief the request handler
    /// @param[in] req the request
    /// @param[out] res the response
    /// @reentrant true //TODO: verify this
    static void requestHandler(qhttp::server::QHttpRequest *req, qhttp::server::QHttpResponse *res);

private slots:

//    /// @brief manage the receive process of the whole request body
//    void requestReceiver();

    /// @brief response to an request
    /// @details the response can only be send in the http server thread
    /// so this slot is needed
    void respond(qhttp::server::QHttpResponse *res, QByteArray data);
};

#endif // HTTPSERVER_H
