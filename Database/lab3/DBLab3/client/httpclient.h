#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "qhttpclient.hpp"
#include "qhttpclientrequest.hpp"
#include "qhttpclientresponse.hpp"

#include "dbrequest.h"
#include "dbresponse.h"
#include <QObject>
#include <QQueue>
#include <QPair>
#include <QMap>

// TODO: add a timeout timer

class HttpClient : public qhttp::client::QHttpClient {
    Q_OBJECT

private:
    /// @brief the pool used to store used descriptors
    QQueue<quint64> tokenPool;
    /// @brief current number of tokens
    quint64 maxTokenNum;

    /// @brief temporarily store server responses until some object retrives it
    /// @details a {token, response} map
    QMap<quint64, DBResponse> responseMap;

    /// @brief cache request to this queue when client is busy (processing a request)
    QQueue<QPair<DBRequest, quint64>> busyCache;
    /// @brief is http client currently sending a request
    bool isBusy;

    /// @brief constructor
    HttpClient();
    HttpClient(const HttpClient &) = delete;
    HttpClient &operator=(const HttpClient &) = delete;

public:
    /// @brief get HttpClient singleton instance
    static HttpClient &getInsatance();

    /// @brief token used to identify request and response
    using Token = QSharedPointer<quint64>;

    /// @brief try send a requset to the server
    /// @param request the request object
    /// @param handler a public slot with the form of handler(DBResponse), just
    /// like slot used in QObject::connect
    /// @return the token used to get data. once token is released, data becomes unavailable
    /// @threadsafe
    Token sendRequest(DBRequest requset);

    /// @brief get response using token
    /// @throw logic_error if the response does not exists
    DBResponse getResponse(Token token);

    // TODO: use tokens(unique number's) or something to manage the connection
signals:
    /// @brief when a response from server is gotten
    /// @param token the token vaule (vaule of pointer returened in sendRequset)
    /// @note connect to this signal use queued connection
    void serverResponsed(quint64 token);

private slots:

    /// @brief internal implemets of sendRequest
    void _sendRequest(DBRequest request, quint64 token);
};

#endif // HTTPCLIENT_H
