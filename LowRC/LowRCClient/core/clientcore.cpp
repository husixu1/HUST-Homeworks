/// @file clientcore.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "clientcore.h"


ClientCore::ClientCore() {
    connector = nullptr;
    transceiver = nullptr;
    _isServerConnected = false;
    connectorListenPort = 0;
    transceiverListenPort = 0;
}

QString ClientCore::getMyEmail() const {
    return myEmail;
}

void ClientCore::setMyEmail(const QString &value) {
    myEmail = value;
}

ClientCore &ClientCore::getInstance() {
    static ClientCore instance;
    return instance;
}

void ClientCore::init() {
    //> init connector
    //>> create connector and thread
    connectorThread.start();
    connector = new BaseConnector;
    connector->moveToThread(&connectorThread);
    //>> auto-delete after thread is deleted
    connect(&connectorThread, SIGNAL(finished()), connector, SLOT(deleteLater()));

    //>> connect start listening related signals
    connect(this, SIGNAL(startListen()),
            connector, SLOT(startListen()),
            Qt::QueuedConnection);
    connect(connector, SIGNAL(listenStarted(QHostAddress, quint16)),
            this, SLOT(connectorListenStarted(QHostAddress, quint16)),
            Qt::QueuedConnection);
    connect(connector, SIGNAL(listenError()),
            this, SLOT(connectorListenFailed()),
            Qt::QueuedConnection);
    //>> initialize connector and start listening
    connector->init();
    emit startListen();

    //>> connect tryConnect related signals
    connect(this, SIGNAL(connectToServer(QSharedPointer<OperationDescriptor>, QHostAddress, quint16)),
            connector, SLOT(tryConnect(QSharedPointer<OperationDescriptor>, QHostAddress, quint16)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(disconnectFromServer(QHostAddress, quint16)),
            connector, SLOT(disconnectHost(QHostAddress, quint16)),
            Qt::QueuedConnection);

    //> init file transfer module
    //>> create and move to thread
    fileThread.start();
    transceiver = new BaseFileTransceiver;
    transceiver->moveToThread(&fileThread);
    //>> auto-delete after thread is deleted
    connect(&fileThread, SIGNAL(finished()), transceiver, SLOT(deleteLater()));
    //>> connect signals
    connect(transceiver, SIGNAL(listenStarted(quint16)),
            this, SLOT(fileListenStarted(quint16)),
            Qt::QueuedConnection);
    connect(transceiver, SIGNAL(failedToListen()),
            this, SLOT(fileListenFailed()),
            Qt::QueuedConnection);
    //>> initialize file transfer
    transceiver->init();
}

const BaseConnector *ClientCore::getConnector() {
    return connector;
}

const BaseFileTransceiver *ClientCore::getFileTransceiver() {
    return transceiver;
}

void ClientCore::connectToServer(QHostAddress serverAddress, quint16 port) {
    if (_isServerConnected) {
        //> disconnect first if already
        emit disconnectFromServer(serverAddress, serverPort());
    }

    this->_serverAddress = serverAddress;
    this->_serverPort = port;

    //> get a new operation descriptor
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    operationMap.insert(operation.data(), operation);

    //> connect signals
    connect(operation.data(), SIGNAL(connected()),
            this, SLOT(serverConnected()));
    connect(operation.data(), SIGNAL(connectError(OperationDescriptor::Error)),
            this, SLOT(serverConnectFailed(OperationDescriptor::Error)));

    //> try to connect to the server
    emit connectToServer(operation, serverAddress, port);
}

bool ClientCore::isServerConnected() {
    return _isServerConnected;
}

QHostAddress ClientCore::serverAddress() {
    return _isServerConnected ? _serverAddress : QHostAddress::Null;
}

quint16 ClientCore::serverPort() {
    return _isServerConnected ? _serverPort : 0;
}

quint16 ClientCore::messagePort() {
    return connectorListenPort;
}

quint16 ClientCore::filePort() {
    return transceiverListenPort;
}

ClientCore::~ClientCore() {
    //> quit the thread and wait for it to quit
    connectorThread.quit();
    fileThread.quit();
    connectorThread.wait();
    fileThread.wait();
}

void ClientCore::connectorListenFailed() {
    qWarning() << "connector failed to listen";
    emit connectorFailedToListen();
}

void ClientCore::connectorListenStarted(QHostAddress address, quint16 port) {
    qInfo() << "Listen started, ip = " << address.toString() << ", port = " << QString::number(port);
    connectorListenPort = port;
    emit connectorListenStarted();
}

void ClientCore::fileListenFailed() {
    qWarning() << "file transceiver failed to listen";
    emit fileListenFailed();
}

void ClientCore::fileListenStarted(quint16 port) {
    qInfo() << "file transceiver listen started, port " << port;
    transceiverListenPort = port;
    emit fileListenStarted();
}

void ClientCore::serverConnected() {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    _isServerConnected = true;
    operationMap.remove(operation);
    emit connectedToServer();
}

void ClientCore::serverConnectFailed(OperationDescriptor::Error error) {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    _isServerConnected = false;
    operationMap.remove(operation);
    emit failedToConnectServer(error);
}

void ClientCore::unexpectedErrorOccurred(QHostAddress address, quint16 port, BaseConnector::UnexpectedError) {
    //> disconnect server if necessary
    if (address == _serverAddress && port == _serverPort )
        _isServerConnected = false;
}
