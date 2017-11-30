/// @file baseconnector.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef BASECONNECTOR_H
#define BASECONNECTOR_H

#include "operationdescriptor.h"
#include "basedataprocessor.h"

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QSharedPointer>
#include <QMap>
#include <QQueue>
#include <QVector>
#include <QPair>
#include <QByteArray>
#include <QTimer>
#include <QtDebug>

#include <cstdlib>

// TODO: switch to singleton mode (make a wrapper for this calss);

// BUG: the upper class may not make a response for data that should have response, thereby making the
// message stay in memory, causing a mem leak
// TODO: split this class into two layer (class),
// the upper for managing send/receive request, the lower for sending data
// [this might never be done since i will not continue this project anymore after the deadline :( ]

// NOTE: this class is a total chaos, i myself will never use it if it's not for the NetworkExperiment :(

/// @brief the implemetation of the connector, used both in server and client
/// provides the function of listening, connecting to socket, receive message from socket
/// @note this object should be run on a thread other than the UI thread
/// @note this class creates a pending receive for all data that should have a response,
/// but will not clear them on timeout. so the peer should ensure that there's
/// a response for them ( or close the socket instead, causing an error, so that
/// the pending receive will be deleted)
class BaseConnector : public QObject {
    Q_OBJECT
public:
    /// @brief message types
    enum DataType {
        /// Request: client -> server
        typeRegisterRequest,
        typeLoginRequest,
        typePassword1Request,
        typePassword2Request,
        typePassword3Request,
        typeMessageRequest,
        typeFileRequest,

        /// Answer: server -> client
        typeRegisterAnswer,
        typeLoginAnswer,
        typePassword1Answer,
        typePassword2Answer,
        typePassword3Answer,
        typeMessageAnswer,
        typeFileAnswer,

        /// Broadcast Server -> client
        typeUserBroadcast,

        //TODO: typeUserBroadcast

        /// @{
        /// @brief client -> client
        typeMessageP2P,
        typeFile1P2P,
        typeFile2P2P,
        /// @}
    };

    /// @brief the unexpected error that could occur
    enum UnexpectedError {
        errConnectionRefused,
        errHostNotFound,
        errSocketAccess,
        errSocketResource,
        errSocketTimedOut,
        errNetwork,
        errUnsupportedSocketOperation,
        errFailedToSendHeartBeat,
        errUnidentifiedError,
    };

private:
    /// @{
    /// @brief the protocols
    static const int headerSize;
    /// @}

    /// @brief the heartBeat package sending send timer
    QSharedPointer<QTimer> heartBeatSendTimer;

    /// @brief some socket auxiliaries
    struct SocketInfo {
        /// @brief the heartbeat receivetimer of the socket
        /// timeout interval is heartBeatTimeOut * 2
        QTimer heartBeatReceiveTimer;
        /// @brief the receive buffer of this socket
        QByteArray receiveBuf;
        /// @brief the expected length of current receive
        int expectingLen;

        /// @brief records the address, used to remove the socket
        QHostAddress address;
        /// @brief records the port, used to remove the socket
        quint16 port;

        /// @brief the pending receive from this socket
        QMap<quint64, QSharedPointer<OperationDescriptor>> pendingReceiveMap;
        /// @brief the pending sent from this socket
        QQueue<QPair<QSharedPointer<OperationDescriptor>, int>> pendingSendQueue;
    };

    /// @brief the timeout of heartBeat/receive package
    static const int heartBeatTimeOut = 10000;

    // some typedefs
    using SocketToSharedPointerMap = QMap<QTcpSocket *, QSharedPointer<QTcpSocket>>;
    using SocketToOperationMap     = QMap<QTcpSocket *, QSharedPointer<OperationDescriptor>>;
    using SocketToSocketInfoMap    = QMap<QTcpSocket *, QSharedPointer<SocketInfo>>;
    using StringToSocketMap        = QMap<QString, QTcpSocket *>;
    using TimerToSocketMap         = QMap<QTimer *, QTcpSocket *>;

    /// @brief map the raw pointer to the shared pointer
    /// also serve as a memory pool to store the QSharedPointer of sockets
    /// @note this should always be the last when trying to remove socket form maps,
    /// since once the socket is removed, the raw pointer goes invalid
    QSharedPointer< SocketToSharedPointerMap > socketPool;

    /// @brief a constant changing map that records sockets that is currently connecting
    /// after the connection of a socket is established, it's remove from this pool
    QSharedPointer< SocketToOperationMap > connectingMap;

    /// @brief all the socket recorded {socket, socketInfo}
    QSharedPointer< SocketToSocketInfoMap > socketMap;
    /// @brief maps the HostAddress.toString + QString::number(port) to socket
    QSharedPointer< StringToSocketMap > addressPortMap;
    /// @brief map the SocketInfo::heartBeatReceiveTimer to the socket
    /// used for quickly finding out the socket when it's timed up
    QSharedPointer< TimerToSocketMap > heartBeatTimerMap;


    /// @brief the socket used to accept connections from other hosts
    QSharedPointer< QTcpServer > acceptor;

    /// @brief if this type of message should have a response
    static bool typeShouldHaveResponse(DataType type);
    /// @brief if this the error is fatal to a socket
    static bool isFatalError(QAbstractSocket::SocketError error);
    /// @brief convert socket error to native UnexpectedError
    static UnexpectedError toUnexpectedError(QAbstractSocket::SocketError);

    /// @brief insert connected socket to maps (socketMap, addressPortMap, heartBeatTimerMap)
    /// @return if the insertion is successful
    /// @note operation not incldue socket pool
    bool insertToMap(QTcpSocket *socket, QSharedPointer<SocketInfo> infoPointer);
    /// @brief remove from maps by socket (socketMap, addressPortMap, heartBeatTimerMap)
    /// @return if removal is successful
    /// @note operation not incldue socket pool
    bool removeFromMap(QTcpSocket *socket);
    /// @brief remove from maps by address and port (socketMap, addressPortMap, heartBeatTimerMap)
    /// @return if removal is successful
    /// @note operation not incldue socket pool
    bool removeFromMap(QHostAddress address, quint16 port);
    /// @brief remove from maps my timeout timer (socketMap, addressPortMap, heartBeatTimerMap)
    /// @return if removal is successful
    /// @note operation not incldue socket pool
    bool removeFromMap(QTimer *timer);

public:
    /// @brief constructor
    BaseConnector();

    /// @brief de-constructor
    ~BaseConnector();

    /// @{
    /// @brief singleton assurance
    BaseConnector(const BaseConnector &) = delete;
    BaseConnector &operator =(const BaseConnector &) = delete;
    /// @}

    ///// @brief get the singleton instanc
    //static BaseConnector &getInstance();

    /// @brief seperate raw data from it's head
    /// @return if the conversion can be performed
    static bool resolveData(
            const QByteArray rawData, BaseConnector::DataType &type,
            quint64 &reqNum, QByteArray &data);

    /// @brief start the connector
    void init();

signals:

    /// @brief error when trying to listen to the port
    /// @note when listenError occurred, this connector is no longer useful. user should delete it
    void listenError();
    /// @brief the connector has started listening, give the host and port
    void listenStarted(QHostAddress, quint16);

    /// @brief host connected successfully
    /// @note this error should be received from operation descriptor
    void forwardConnected(QSharedPointer<OperationDescriptor>);
    /// @brief error when trying to connecte
    /// @note this error should be received from operation descriptor
    void forwardConnectError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error);

    /// @brief data sent successfully
    /// @note this error should be received from operation descriptor
    void fowardDataSent(QSharedPointer<OperationDescriptor>);
    /// @brief expected data received successfully
    /// the data received is raw data, use resolveData() to seperate it
    /// @note This error should be received from operation descriptor.
    void forwardDataReceived(QSharedPointer<OperationDescriptor>, QByteArray);
    /// @brief error when trying to send/receive
    /// @note this error should be received from operation descriptor
    void forwardSendReceiveError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error);

    /// @brief send the unexpected (started by others) data to the receiver
    void unexpectedDataReceived(QHostAddress address, quint16 port, QByteArray data);
    /// @brief indicates that an unexpected error has occurred (will close the socket at most of the time)
    /// the data received is raw data, use resolveData() to seperate it
    void unexpectedError(QHostAddress, quint16, BaseConnector::UnexpectedError error);


private slots:
    /// @brief initialize this object
    void initialize();

    /// @brief receive the socket error and fowrard it according to different socket states
    void error(QAbstractSocket::SocketError);
    /// @brief when the socket is connected, add it to maps, sets etc.
    void connected();
    /// @brief when a incoming data (from socket) arrives
    void receiveData();
    /// @brief when some bytes are sent by a socket
    void bytesSent(qint64);
    /// @brief when a socket hasn't been receiving data/heartbeat for heartBeatTimeOut seconds
    void socketTimedOut();

    /// @brief send heartBeat to all the connected sockets
    void sendHeartBeat();

    /// @brief a new incoming connection has arrived
    void newConnection();

// for debug
#ifdef QT_DEBUG
    /// @brief print the connected sockets
    void printSockets();
#endif

public slots:

    /// @brief start listening on random addr/port
    void startListen();

    /// @brief start listening on specific port
    void startListen(quint16 port);

    /// @brief connect to host
    void tryConnect(QSharedPointer<OperationDescriptor> operation,
            QHostAddress address, quint16 port);

    /// @brief send data to designated destination
    /// @note the request number should be guranteed unique by user,
    /// operation->getId() can be used, since it's guranteed unique globally
    void trySendData(QSharedPointer<OperationDescriptor> operation,
            QHostAddress address, quint16 port,
            quint64 reqNum, BaseConnector::DataType type, QByteArray data);

    /// @brief disconnect from host ip:port
    void disconnectHost(QHostAddress address, quint16 port);
};

// NOTE: these should be added in the constructor too
Q_DECLARE_METATYPE(QHostAddress)
Q_DECLARE_METATYPE(QSharedPointer<OperationDescriptor>)
Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(OperationDescriptor::Error)
Q_DECLARE_METATYPE(BaseConnector::DataType)
Q_DECLARE_METATYPE(BaseConnector::UnexpectedError)

#endif // BASECONNECTOR_H
