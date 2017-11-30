/// @file baseconnector.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "baseconnector.h"

const int BaseConnector::headerSize = 19;

#ifdef QT_DEBUG
#include <QTextStream>
QTextStream out(stdout);
#endif


//TODO: add heartBeat
BaseConnector::BaseConnector() {
    //> register types needed
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<QByteArray>("QByteArray");
    qRegisterMetaType<QSharedPointer<OperationDescriptor>>();
    qRegisterMetaType<OperationDescriptor::Error>();
    qRegisterMetaType<BaseConnector::DataType>();
    qRegisterMetaType<BaseConnector::UnexpectedError>();
}

BaseConnector::~BaseConnector() {
    // must be clearing by sequence
    // (otherwise data might be written to socket while socket is already closed)
    connectingMap->clear();
    socketMap->clear();
    addressPortMap->clear();
    heartBeatTimerMap->clear();
    // this must be placed at last
    socketPool->clear();
}

bool BaseConnector::resolveData(
        const QByteArray rawData, BaseConnector::DataType &type,
        quint64 &reqNum, QByteArray &data) {
    using namespace std;

    // the size of raw data but be larger than header
    if (rawData.size() < headerSize)
        return false;

    //> copy data, reqnum and raw type
    memcpy(&reqNum, rawData.left(8).data(), 8);
    data = rawData.mid(headerSize, rawData.size());
    QString rawType = rawData.mid(8, 3);

    // the conversion table
    static const QMap<QString, DataType> convertMap = {
        {"RR1", typeRegisterRequest},   {"RA1", typeRegisterAnswer},
        {"LR1", typeLoginRequest},      {"LA1", typeLoginAnswer},
        {"PR1", typePassword1Request},  {"PA1", typePassword1Answer},
        {"PR2", typePassword2Request},  {"PA2", typePassword2Answer},
        {"PR3", typePassword3Request},  {"PA3", typePassword3Answer},
        {"MR1", typeMessageRequest},    {"PA1", typeMessageAnswer},
        {"FR1", typeFileRequest},       {"FR2", typeFileAnswer},

        {"UB1", typeUserBroadcast},

        {"MC1", typeMessageP2P},
        {"FC1", typeFile1P2P},
        {"FC2", typeFile2P2P},
    };

    //> convert type
    if (! convertMap.contains(rawType))
        return false;
    type = convertMap[rawType];
    return true;
}


bool BaseConnector::typeShouldHaveResponse(BaseConnector::DataType type) {
    return  type == typeRegisterRequest ||
            type == typeLoginRequest ||
            type == typePassword1Request ||
            type == typePassword2Request ||
            type == typePassword3Request ||
            type == typeMessageRequest ||
            type == typeFileRequest ||
            type == typeFile1P2P ;
}

bool BaseConnector::isFatalError(QAbstractSocket::SocketError error) {
    // a set of fatal errors
    static const QSet<QAbstractSocket::SocketError> fatalErrorSet = {
        QAbstractSocket::ConnectionRefusedError,
        QAbstractSocket::HostNotFoundError,
        QAbstractSocket::SocketAccessError,
        QAbstractSocket::SocketResourceError,
        QAbstractSocket::SocketTimeoutError,
        QAbstractSocket::NetworkError,
        QAbstractSocket::SocketAddressNotAvailableError,
    };
    return fatalErrorSet.contains(error);
}

BaseConnector::UnexpectedError BaseConnector::toUnexpectedError(QAbstractSocket::SocketError error) {
    // map unexpected socket error to native UnexpectedError
    static const QMap<QAbstractSocket::SocketError, UnexpectedError> errorMap = {
        {QAbstractSocket::ConnectionRefusedError,          errConnectionRefused},
        {QAbstractSocket::HostNotFoundError,               errHostNotFound},
        {QAbstractSocket::SocketAccessError,               errSocketAccess},
        {QAbstractSocket::SocketResourceError,             errSocketResource},
        {QAbstractSocket::SocketTimeoutError,              errSocketTimedOut},
        {QAbstractSocket::NetworkError,                    errNetwork},
        {QAbstractSocket::UnsupportedSocketOperationError, errUnsupportedSocketOperation},
    };
    if (errorMap.contains(error))
        return errorMap[error];
    else
        return errUnidentifiedError;
}

bool BaseConnector::insertToMap(
        QTcpSocket *socket,
        QSharedPointer<BaseConnector::SocketInfo> infoPointer) {

    QString addressPort = socket->peerAddress().toString() + QString::number(socket->peerPort());
    infoPointer->address = socket->peerAddress();
    infoPointer->port = socket->peerPort();

    // if any of the key is already in the pool
    if ( socketMap->contains(socket) ||
            addressPortMap->contains(addressPort) ||
            heartBeatTimerMap->contains(&(infoPointer->heartBeatReceiveTimer)) ) {
        return false;
    }

    //> insert all info
    socketMap->insert(socket, infoPointer);
    addressPortMap->insert(addressPort, socket);
    heartBeatTimerMap->insert(&(infoPointer->heartBeatReceiveTimer), socket);
    return true;
}

bool BaseConnector::removeFromMap(QTcpSocket *socket) {
    // socket must be in the map
    if (!socketMap->contains(socket))
        return false;

    // remove from maps, using the port before disconnection
    addressPortMap->remove((*socketMap)[socket]->address.toString()
            + QString::number((*socketMap)[socket]->port));
    heartBeatTimerMap->remove( &((*socketMap)[socket]->heartBeatReceiveTimer) );
    socketMap->remove(socket);
    return true;
}

bool BaseConnector::removeFromMap(QHostAddress address, quint16 port) {
    QString addressPort = address.toString() + QString::number(port);

    // socket must be in the map
    if (!addressPortMap->contains(addressPort))
        return false;

    //> find the socket and remove from maps
    QTcpSocket *socket = (*addressPortMap)[addressPort];
    return removeFromMap(socket);
}

bool BaseConnector::removeFromMap(QTimer *timer) {
    if (!heartBeatTimerMap->contains(timer))
        return false;

    //> find the socket and remove from maps
    QTcpSocket *socket = (*heartBeatTimerMap)[timer];
    return removeFromMap(socket);
}

//BaseConnector &BaseConnector::getInstance() {
//    static BaseConnector instance;
//    return instance;
//}

void BaseConnector::init() {
    QThread *objectThread = this->thread();
    QThread *currentThread = QThread::currentThread();

    //> this object must be inited in another thread
    Q_ASSERT (objectThread != currentThread);

    //> execute in the thread that this object is running on
    QMetaObject::invokeMethod(this, "initialize", Qt::QueuedConnection);
}

void BaseConnector::initialize() {
    //> alloc space (so that members and this are destroyed in the same thread)
    acceptor           = QSharedPointer<QTcpServer>(new QTcpServer(this));
    socketPool         = QSharedPointer<SocketToSharedPointerMap>(new SocketToSharedPointerMap);
    connectingMap      = QSharedPointer<SocketToOperationMap>(new SocketToOperationMap);
    socketMap          = QSharedPointer<SocketToSocketInfoMap>(new SocketToSocketInfoMap);
    addressPortMap     = QSharedPointer<StringToSocketMap>(new StringToSocketMap);
    heartBeatTimerMap  = QSharedPointer<TimerToSocketMap>(new TimerToSocketMap);
    heartBeatSendTimer = QSharedPointer<QTimer>(new QTimer);

    //> start sending heartBeats
    connect(heartBeatSendTimer.data(), SIGNAL(timeout()), this, SLOT(sendHeartBeat()));
    heartBeatSendTimer->start(heartBeatTimeOut);
}

void BaseConnector::connected() {
    //> get the sender
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    Q_ASSERT(socket);

    //> move the socket to the socket pool, add to maps
    QSharedPointer<SocketInfo> info(new SocketInfo);
    insertToMap(socket, info);

    //> connect signals and start heartbeating
    connect(&(info->heartBeatReceiveTimer), SIGNAL(timeout()),
            this, SLOT(socketTimedOut()));
    info->heartBeatReceiveTimer.start(heartBeatTimeOut * 2);

    //> emit connected signal and remove from connected map
    QSharedPointer<OperationDescriptor> operation = (*connectingMap)[socket];
    connectingMap->remove(socket);
    emit forwardConnected(operation);
}

void BaseConnector::error(QAbstractSocket::SocketError error) {
    //> get the sender
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    Q_ASSERT(socketPool->contains(socket));

    // judge whether the socket is connecting or connected
    if (connectingMap->contains(socket)) {
        emit forwardConnectError((*connectingMap)[socket], OperationDescriptor::translateSocketError(error));
        connectingMap->remove(socket);
        socketPool->remove(socket);
    } else {
        // if this is a while sending and receiveing
        if (!(*socketMap)[socket]->pendingSendQueue.empty() || !(*socketMap)[socket]->pendingReceiveMap.empty()) {
            //> send to all pending send and receives in the queue and close the socket
            socket->close();

            for (auto item : (*socketMap)[socket]->pendingSendQueue)
                emit forwardSendReceiveError(item.first, OperationDescriptor::translateSocketError(error));

            for (auto operation : (*socketMap)[socket]->pendingReceiveMap)
                emit forwardSendReceiveError(operation, OperationDescriptor::translateSocketError(error));

            //> delete all pending sends and receives
            (*socketMap)[socket]->pendingReceiveMap.clear();
            (*socketMap)[socket]->pendingSendQueue.clear();
        }

        // if it's a fatal error, emit unexpectedError and relase the socket related resources
        if (isFatalError(error)) {
            emit unexpectedError((*socketMap)[socket]->address, (*socketMap)[socket]->port, toUnexpectedError(error));
            socket->close();

            //> remove the socket from map
            removeFromMap(socket);
            socketPool->remove(socket);
        }
    }
}

void BaseConnector::receiveData() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    Q_ASSERT(socket && socketMap->contains(socket));

    // if no data but a readyRead is triggered
    if (socket->bytesAvailable() == 0)
        return;

    QSharedPointer<SocketInfo> info = (*socketMap)[socket];

    // the request number, datasize and datatype
    quint64 temp;
    quint64 reqNum;
    QByteArray type;

    while (socket->bytesAvailable() != 0) {
        qDebug() << "available " << socket->bytesAvailable();
        // wait for the whole header to arrive
        if (info->receiveBuf.size() < headerSize)
            info->receiveBuf.append(socket->read(headerSize - info->receiveBuf.size()));

        // if still not received the header
        if (info->receiveBuf.size() < headerSize)
            continue;

        // exactly the header has been received
        if (!info->receiveBuf.isEmpty() && info->receiveBuf.size() == headerSize) {
            //> set the size of the data
            memcpy(&temp, info->receiveBuf.mid(11, 8).data(), 8);
            info->expectingLen = temp;
            //> get the reqNum
            memcpy(&reqNum, info->receiveBuf.left(8).data(), 8);
            //> get the message type
            type = info->receiveBuf.mid(8, 3);

            // heartBeat package
            if (type == QString("HB1")) {
                //> restart the timer
                (*socketMap)[socket]->heartBeatReceiveTimer.start();
                qDebug() << "heartBeat received";

                //> clear the buffer and wait for the next message
                info->expectingLen = 0;
                info->receiveBuf.clear();
                continue;
            }
        }

        // try to read the whole message
        if (info->expectingLen && (info->receiveBuf.size() < info->expectingLen + headerSize)) {
            info->receiveBuf.append(socket->read(info->expectingLen + headerSize - info->receiveBuf.size()));
        }

        // if the whole message is recieved
        if (info->expectingLen && (info->receiveBuf.size() == info->expectingLen + headerSize)) {
            //> if this is a pending message, send that the message is received
            if (info->pendingReceiveMap.contains(reqNum)) {
                emit forwardDataReceived(info->pendingReceiveMap[reqNum], info->receiveBuf);
                info->pendingReceiveMap.remove(reqNum);
            } else {
                //> give the data to the processor
                emit unexpectedDataReceived(socket->peerAddress(), socket->peerPort(), info->receiveBuf);
            }

            //> clear the buffer and wait for the next message
            info->expectingLen = 0;
            info->receiveBuf.clear();
        }
    }
}

void BaseConnector::bytesSent(qint64 bytes) {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    Q_ASSERT(socket);
    QSharedPointer<SocketInfo> info = (*socketMap)[socket];

    // pop the send queue while the bytes send is greater than message size
    while (!info->pendingSendQueue.empty() && bytes >= info->pendingSendQueue.front().second) {
        emit fowardDataSent(info->pendingSendQueue.front().first);
        bytes -= info->pendingSendQueue.front().second;
        info->pendingSendQueue.pop_front();
    }

    if (!info->pendingSendQueue.empty()) {
        //> part of the message is send, set the pending send size to the remaining size
        info->pendingSendQueue.front().second -= bytes;
    }
}

void BaseConnector::socketTimedOut() {
    QTimer *timer = qobject_cast<QTimer *>(sender());
    Q_ASSERT(timer);

    QTcpSocket *socket = (*heartBeatTimerMap)[timer];

    // if this is a while sending and receiveing
    if (socketMap->contains(socket)) {
        if (!(*socketMap)[socket]->pendingSendQueue.empty() || !(*socketMap)[socket]->pendingReceiveMap.empty()) {
            //> send to all pending send and receives in the queue and close the socket
            socket->close();

            for (auto item : (*socketMap)[socket]->pendingSendQueue)
                emit forwardSendReceiveError(item.first, OperationDescriptor::errSocketTimedOut);

            for (auto operation : (*socketMap)[socket]->pendingReceiveMap)
                emit forwardSendReceiveError(operation, OperationDescriptor::errSocketTimedOut);

            //> delete all pending sends and receives
            (*socketMap)[socket]->pendingReceiveMap.clear();
            (*socketMap)[socket]->pendingSendQueue.clear();
        }
    }

    emit unexpectedError((*socketMap)[socket]->address, (*socketMap)[socket]->port, errSocketTimedOut);

    //> remove from pool
    removeFromMap(timer);
    socketPool->remove(socket);
}

void BaseConnector::sendHeartBeat() {
    QByteArray heartBeatPack(headerSize, 0);
    heartBeatPack.replace(8, 3, "HB1");

    // a temporary operation descriptor for heart beating
    static QSharedPointer<OperationDescriptor> heartOp = OperationDescriptor::getNewOperationDescriptor();

    // the socket in socketMap is automatically considered connected, send heartbeat
    for (auto socket : socketMap->keys()) {
        // is the socket/info is no longer valid
        if ((*socketMap)[socket].isNull()) {
            emit unexpectedError((*socketMap)[socket]->address, (*socketMap)[socket]->port, errFailedToSendHeartBeat);
            continue;
        }
        socketMap.isNull();

        (*socketMap)[socket]->pendingSendQueue.push_back({heartOp, headerSize});

        int totalWritten = 0, bytesWritten;
        //> do general write process
        while (totalWritten < headerSize) {
            bytesWritten = socket->write(heartBeatPack.right(heartBeatPack.size() - totalWritten));
            if (bytesWritten == -1) {
                emit unexpectedError((*socketMap)[socket]->address, (*socketMap)[socket]->port, errFailedToSendHeartBeat);
                removeFromMap(socket);
                socketPool->remove(socket);
                break;
            }
            totalWritten += bytesWritten;
        }
        qDebug() << "heartBeat send to " << socket;
    }
}

void BaseConnector::newConnection() {
    while (acceptor->hasPendingConnections()) {
        //> accept the connections and add to maps
        QTcpSocket *socket = acceptor->nextPendingConnection();

        //> if socket already exists, replace it
        if (addressPortMap->contains(socket->peerAddress().toString() + QString::number(socket->peerPort())) ) {
            QString addrPort = socket->peerAddress().toString() + QString::number(socket->peerPort());
            QTcpSocket *temp = (*addressPortMap)[addrPort];

            //> remove form maps and pool
            removeFromMap(socket->peerAddress(), socket->peerPort());
            socketPool->remove(temp);
        }

        /// @quotation It is still a good idea to delete the object explicitly
        /// when you are done with it, to avoid wasting memory. -- Qt5 Document
        QSharedPointer<QTcpSocket> socketPointer(socket);
        QSharedPointer<SocketInfo> info(new SocketInfo);

        //> insert allocated objects to maps
        socketPool->insert(socket, socketPointer);
        insertToMap(socket, info);

        //> connect the signals
        connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesSent(qint64)));
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
        connect(&(info->heartBeatReceiveTimer), SIGNAL(timeout()),
                this, SLOT(socketTimedOut()));

        //> start receiveing heartbeat
        info->heartBeatReceiveTimer.start(heartBeatTimeOut * 2);

        qDebug() << "new incoming connection: " << socket->peerAddress().toString() << "_" << socket->peerPort() << endl;
    }
}

void BaseConnector::startListen() {
    if (acceptor->listen()) {
        //> connect signals
        connect(acceptor.data(), SIGNAL(newConnection()), this, SLOT(newConnection()), Qt::UniqueConnection);
        emit listenStarted(acceptor->serverAddress(), acceptor->serverPort());
    } else {
        emit listenError();
    }
}

void BaseConnector::startListen(quint16 port) {
    if (acceptor->listen(QHostAddress::Any, port)) {
        connect(acceptor.data(), SIGNAL(newConnection()), this, SLOT(newConnection()), Qt::UniqueConnection);
        emit listenStarted(acceptor->serverAddress(), acceptor->serverPort());
    } else {
        emit listenError();
    }
}

//void BaseConnector::pendingReceiveTimedOut() {
//    QSharedPointer<QTimer> timer(qobject_cast<QTimer *>(sender()));
//    Q_ASSERT(! timer.isNull() && pendingReceiveTimerMap.contains(timer));
//
//    //> get the operation, emit error signal, remove from maps
//    QSharedPointer<OperationDescriptor> operation = pendingReceiveTimerMap[timer];
//    emit connectorError(operation, OperationDescriptor::errPendingReceiveTimedOut);
//    pendingReceiveMap.remove(operation);
//    pendingReceiveTimerMap.remove(timer);
//}


void BaseConnector::tryConnect(
        QSharedPointer<OperationDescriptor> operation,
        QHostAddress address, quint16 port) {

    // connect operation descriptor
    if (!operation.isNull()) {
        connect(this, SIGNAL(forwardConnectError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                operation.data(), SLOT(forwardConnectError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(forwardConnected(QSharedPointer<OperationDescriptor>)),
                operation.data(), SLOT(forwardConnected(QSharedPointer<OperationDescriptor>)),
                Qt::QueuedConnection);
    }

    // if already connected
    if (addressPortMap->contains(address.toString() + QString::number(port))) {
        emit forwardConnected(operation);
        return;
    }

    // create a socket and try to connect to the host
    QTcpSocket *socket = new QTcpSocket;

    // connect signals
    connect(socket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(receiveData()));
    connect(socket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(bytesSent(qint64)));

    socketPool->insert(socket, QSharedPointer<QTcpSocket>(socket));
    connectingMap->insert(socket, operation);
    socket->connectToHost(address, port);
}

#ifdef QT_DEBUG
void BaseConnector::printSockets() {
    out << "vvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
    out << "Socket Pool -------------" << endl;
    for (auto i : socketPool->keys())
        out << i << "|" << i->peerAddress().toString() << "|" << QString::number(i->peerPort()) << endl;

    out << "Connecting Map ----------" << endl;
    for (auto i : connectingMap->keys())
        out << i << "|" << i->peerAddress().toString() << "|" << QString::number(i->peerPort()) << endl;

    out << "Socket Map --------------" << endl;
    for (auto i : socketMap->keys()) {
        out << i << "|" << i->peerAddress().toString() << "|" << QString::number(i->peerPort())
                << endl << "\tpendingSendQueue: ";

        for (auto j : (*socketMap)[i]->pendingSendQueue)
            out << j.second << "-";
        out << endl << "\tpendingReceiveMap: ";

        for (auto j : (*socketMap)[i]->pendingReceiveMap.keys())
            out << j << "-";
        out << endl;
    }

    out << "^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
}
#endif

void BaseConnector::trySendData(
        QSharedPointer<OperationDescriptor> operation,
        QHostAddress address, quint16 port,
        quint64 reqNum, BaseConnector::DataType type, QByteArray data) {

    static const QMap<BaseConnector::DataType, QByteArray> dataTypeMap = {
        {typeRegisterRequest, "RR1"},   {typeRegisterAnswer, "RA1"},
        {typeLoginRequest, "LR1"},      {typeLoginAnswer, "LA1"},
        {typePassword1Request, "PR1"},  {typePassword1Answer, "PA1"},
        {typePassword2Request, "PR2"},  {typePassword2Answer, "PA2"},
        {typePassword3Request, "PR3"},  {typePassword3Answer, "PA3"},
        {typeMessageRequest, "MR1"},    {typeMessageAnswer, "MA1"},
        {typeFileRequest, "FR1"},       {typeFileAnswer, "FA1"},

        {typeUserBroadcast, "UB1"},
        {typeMessageP2P, "MC1"},
        {typeFile1P2P, "FC1"},
        {typeFile2P2P, "FC2"},
    };

    //> connect this and operation
    if (!operation.isNull()) {
        connect(this, SIGNAL(fowardDataSent(QSharedPointer<OperationDescriptor>)),
                operation.data(), SLOT(forwardDataSent(QSharedPointer<OperationDescriptor>)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(forwardDataReceived(QSharedPointer<OperationDescriptor>, QByteArray)),
                operation.data(), SLOT(forwardDataReceived(QSharedPointer<OperationDescriptor>, QByteArray)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(forwardSendReceiveError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                operation.data(), SLOT(forwardSendReceiveError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                Qt::QueuedConnection);
    }

    // if the socket is not found
    /// @quotation QMap::operator[]: the function inserts a
    /// default-constructed value into the map with key key,
    /// and returns a reference to it. - Qt 5 document
    if (!addressPortMap->contains(address.toString() + QString::number(port))) {
        emit forwardSendReceiveError(operation, OperationDescriptor::errNotConnected);
        return;
    }

    //> get the socket
    QTcpSocket *socket = (*addressPortMap)[address.toString() + QString::number(port)];
    QSharedPointer<SocketInfo> info = (*socketMap)[socket];

    // is the socket/info is no longer valid
    if (!socket || info.isNull()) {
        emit forwardSendReceiveError(operation, OperationDescriptor::errInternal);
        return;
    }

    //> malloc space
    QByteArray temp(data.size() + 19, 0);

    // the buffer used to convert integers
    char convertBuf[8];

    memcpy(convertBuf, &reqNum, 8);
    temp.replace(0, 8, convertBuf, 8);

    temp.replace(8, 3, dataTypeMap[type]);

    //> set data length
    quint64 msgSize = static_cast<quint64>(data.size());
    memcpy(convertBuf, &msgSize, 2);
    temp.replace(11, 8, convertBuf, 8);

    //> copy the messages
    temp.replace(19, data.size(), data);

    //> if the message sent should have a response
    if (typeShouldHaveResponse(type)) {
        //> add to pending recieve map
        info->pendingReceiveMap.insert(reqNum, operation);
    }

    // send data
    info->pendingSendQueue.push_back({operation, temp.size()});

    int totalWritten = 0, bytesWritten;
    while (totalWritten < temp.size()) {
        // add to totalWritten
        bytesWritten = socket->write(temp.right(temp.size() - totalWritten));
        // error occurred, close the socket
        if (bytesWritten == -1) {
            emit forwardSendReceiveError(operation, OperationDescriptor::errInternal);
            socket->close();

            //> remove socke from maps
            removeFromMap(socket);
            socketPool->remove(socket);
            break;
        }
        totalWritten += bytesWritten;
    }
    qDebug() << "data sent to " << socket;
}

void BaseConnector::disconnectHost(QHostAddress address, quint16 port) {
    QString addressPort = address.toString() + QString::number(port);
    if (addressPortMap->contains(addressPort)) {
        QTcpSocket *socket = (*addressPortMap)[address.toString() + QString::number(port)];
        socket->close();
        //TODO change to function
        addressPortMap->remove(addressPort);
        heartBeatTimerMap->remove(&((*socketMap)[socket]->heartBeatReceiveTimer));
        socketMap->remove(socket);
        socketPool->remove(socket);
    }
}
