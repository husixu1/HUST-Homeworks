/// @file connector.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "connector.h"

Connector::~Connector() {
    thread.quit();
    thread.wait();
}

void Connector::init() {
    thread.start();
    BaseConnector::getInstance().moveToThread(&thread);

    //> connect signals
    connect(&BaseConnector::getInstance(), SIGNAL(connected(QSharedPointer<OperationDescriptor>)),
            this, SIGNAL(connected(QSharedPointer<OperationDescriptor>)),
            Qt::QueuedConnection);
    connect(&BaseConnector::getInstance(), SIGNAL(connectError(QSharedPointer<OperationDescriptor>,OperationDescriptor::Error)),
            this, SIGNAL(connectError(QSharedPointer<OperationDescriptor>,OperationDescriptor::Error)),
            Qt::QueuedConnection);
    connect(&BaseConnector::getInstance(), SIGNAL(dataReceived(QSharedPointer<OperationDescriptor>)),
            this, SIGNAL(dataReceived(QSharedPointer<OperationDescriptor>)),
            Qt::QueuedConnection);
    connect(&BaseConnector::getInstance(), SIGNAL(dataSent(QSharedPointer<OperationDescriptor>)),
            this, SIGNAL(dataSent(QSharedPointer<OperationDescriptor>)),
            Qt::QueuedConnection);
    connect(&BaseConnector::getInstance(), SIGNAL(listenError()),
            this, SIGNAL(listenError()),
            Qt::QueuedConnection);
    connect(&BaseConnector::getInstance(), SIGNAL(listenStarted(QHostAddress,quint16)),
            this, SIGNAL(listenStarted(QHostAddress,quint16))
            Qt::QueuedConnection);
    connect(&BaseConnector::get)
}
