/// @file clientcore.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include "baseconnector.h"
#include "basefiletransceiver.h"
#include "operationdescriptor.h"

#include <QObject>
#include <QHostAddress>
#include <QThread>
#include <QSharedPointer>
#include <QDebug>

/// @brief the class that handles par of the connector and transceiver's action
/// this class handles the initialize, listen action of the BaseConnector and BaseFileTransceiver
/// it's also used to connect to the server using BaseConnector. But it does not handle the usage
/// of BaseConnector and BaseFileTransceiver, user should use it directly through the two class' signal and slor
class ClientCore : public QObject {
    Q_OBJECT
private:
    /// @brief constructor
    ClientCore();

    /// @brief only one connector per client
    BaseConnector *connector;
    /// @brief the thread that this connector is running on
    QThread connectorThread;

    /// @brief only one file transceiver per client
    BaseFileTransceiver *transceiver;
    /// @brief the thread that this file transceiver is running on
    QThread fileThread;


    /// @brief the connecotor's listening port
    quint16 connectorListenPort;
    /// @brief the transceiver's listening port
    quint16 transceiverListenPort;

    /// @brief the address of the server
    /// this variable is automatically inited by tryConnectServer()
    QHostAddress _serverAddress;
    /// @brief the port of the server
    /// this variable is automatically inited by tryConnectServer()
    quint16 _serverPort;
    /// @brief is the server connected
    bool _isServerConnected;

    /// @brief a pool that stops the operation
    QMap<OperationDescriptor *, QSharedPointer<OperationDescriptor>> operationMap;

    /// @brief the email of myself
    QString myEmail;

public:
    /// @brief get the singleton instance
    static ClientCore &getInstance();

    /// @brief initialize BaseConnector, and record server addr/port
    void init();

    /// @brief get the connector
    const BaseConnector *getConnector();

    /// @brief get the file transceiver
    const BaseFileTransceiver *getFileTransceiver();

    /// @brief try to connect to the server
    /// try to connect to server at serverAddress:port
    /// the succeeded or failed signal should be received from
    /// the connector returned by getConnector().
    /// if the server is connected, the server is disconnected and re-connected
    void connectToServer(QHostAddress serverAddress, quint16 port);

    /// @brief return if the server is connected
    bool isServerConnected();

    /// @brief return the server's address, or QHostAddress::Null if not connected
    QHostAddress serverAddress();
    /// @brief return the server's port, or 0 if not connected
    quint16 serverPort();

    /// @brief get the connector's listening port
    quint16 messagePort();
    /// @brief get the transceiver's listening port
    quint16 filePort();

    ClientCore(const ClientCore &) = delete;
    /// @brief de-constructor
    ~ClientCore();

    QString getMyEmail() const;
    void setMyEmail(const QString &value);

signals:
    /// @brief signal sent to baseConnector, do not use
    void startListen();

    /// @brief the connecter started listen successfully
    void connectorListenStarted();
    /// @brief the connector failed to start listen
    void connectorFailedToListen();

    /// @brief the file transceiver started to listen
    void fileListenStarted();
    /// @brief the file transceiver failed to listen
    void fileFailedToListen();

    /// @brief emitted when the ClientCore is trying to connect to the server
    /// @note this signal should not be used by the UI
    void connectToServer(QSharedPointer<OperationDescriptor> operation, QHostAddress address, quint16 port);

    /// @brief emitted when the ClientCore is trying to disconnect from server
    /// @note this signal should not be used by the UI
    void disconnectFromServer(QHostAddress, quint16);

    /// @brief emitted when server is connected
    void connectedToServer();
    /// @brief emitted when the connection to server failed
    void failedToConnectServer(OperationDescriptor::Error error);

private slots:
    /// @brief when the connector failed to listen
    void connectorListenFailed();
    /// @brief when the connector started to listen
    void connectorListenStarted(QHostAddress address, quint16 port);

    /// @brief the file transceiver failed to start listen
    void fileListenFailed();
    /// @brief the file transceiver listen started
    void fileListenStarted(quint16 port);

    /// @brief the connection to the server is successful
    void serverConnected();
    /// @brief the connection to the server failed
    void serverConnectFailed(OperationDescriptor::Error error);

    /// @brief when an unexpected error occurred
    /// this slot is used to filter the server connection error, and set isServerConnected false;
    void unexpectedErrorOccurred(QHostAddress address, quint16 port, BaseConnector::UnexpectedError error);
};

#endif // CLIENTCORE_H
