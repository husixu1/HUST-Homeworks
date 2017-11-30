/// @file servercore.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "servercore.h"

ServerCore &ServerCore::getInstance() {
    static ServerCore instance;
    return instance;
}

void ServerCore::init() {
    //> init connector
    //>> create connector and thread
    connectorThread.start();
    connector = new BaseConnector();
    connector->moveToThread(&connectorThread);
    //>> auto-delete after thread is deleted
    connect(&connectorThread, SIGNAL(finished()), connector, SLOT(deleteLater()));
    //>> initialize connector
    connector->init();
    //>> start listening

    connect(this, SIGNAL(startListen(quint16)),
            connector, SLOT(startListen(quint16)),
            Qt::QueuedConnection);
    connect(connector, SIGNAL(listenStarted(QHostAddress, quint16)),
            this, SLOT(listenStarted(QHostAddress, quint16)),
            Qt::QueuedConnection);
    connect(connector, SIGNAL(listenError()),
            this, SLOT(listenFailed()),
            Qt::QueuedConnection);
    emit startListen(serverPort);

    //> init processor
    //>> create processor and thread
    processorThread.start();
    processor = new DataProcessor();
    processor->moveToThread(&processorThread);
    //>> auto-delete after thread is deleted
    connect(&processorThread, SIGNAL(finished()), processor, SLOT(deleteLater()));

    //> TODO: connect signals
    connect(connector, SIGNAL(unexpectedDataReceived(QHostAddress, quint16, QByteArray)),
            processor, SLOT(processData(QHostAddress, quint16, QByteArray)),
            Qt::QueuedConnection);

    connect(connector, SIGNAL(unexpectedError(QHostAddress, quint16, BaseConnector::UnexpectedError)),
            processor, SLOT(unexpectedError(QHostAddress, quint16, BaseConnector::UnexpectedError)),
            Qt::QueuedConnection);

    connect(processor, SIGNAL(sendData(QSharedPointer<OperationDescriptor>, QHostAddress, quint16, quint64, BaseConnector::DataType, QByteArray)),
            connector, SLOT(trySendData(QSharedPointer<OperationDescriptor>, QHostAddress, quint16, quint64, BaseConnector::DataType, QByteArray)),
            Qt::QueuedConnection);
}

ServerCore::~ServerCore() {
    // WARNING TEST
    connectorThread.quit();
    processorThread.quit();
    connectorThread.wait();
    processorThread.wait();
}

const BaseConnector *ServerCore::getConnector() {
    return connector;
}

void ServerCore::listenFailed() {
    qInfo() << "failed to start listening" << endl;
}

void ServerCore::listenStarted(QHostAddress address, quint16 port) {
    qInfo() << "listen started, address: " << address.toString() << ", port: " << QString::number(port) << endl;
}


