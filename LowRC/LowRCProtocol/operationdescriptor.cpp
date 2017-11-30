/// @file operationdescriptor.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "operationdescriptor.h"

/// @brief alloc space for the pool
QQueue<quint64> OperationDescriptor::idPool;
quint64             OperationDescriptor::idNum = 0;

QSharedPointer<OperationDescriptor> OperationDescriptor::getNewOperationDescriptor() {
    QSharedPointer<OperationDescriptor> newOne(new OperationDescriptor);
    return newOne;
}

OperationDescriptor::OperationDescriptor() {
    // get an unique id from the pool
    if (!idPool.empty()) {
        //> get a id if pool is not empty
        id = idPool.front();
        idPool.pop_front();
    } else {
        //> generate new id if pool is not empty, id starts from 1 (when idNum is 0)
        ++idNum;
        id = idNum;
    }
}

OperationDescriptor::~OperationDescriptor() {
    // return the id to the pool
    if (id == idNum) {
        --idNum;
    } else {
        idPool.push_back(id);
    }
}

quint64 OperationDescriptor::getId() {
    return id;
}

bool OperationDescriptor::operator ==(const OperationDescriptor &a) {
    return a.id == id;
}

bool OperationDescriptor::operator !=(const OperationDescriptor &a) {
    return a.id != id;
}

OperationDescriptor::Error OperationDescriptor::translateSocketError(QAbstractSocket::SocketError error) {
    static const QMap<QAbstractSocket::SocketError, Error> errorMap = {
        {QAbstractSocket::ConnectionRefusedError,   errConnectionRefused},
        {QAbstractSocket::RemoteHostClosedError,    errRemoteHostClosed},
        {QAbstractSocket::HostNotFoundError,        errHostNotFound},
        {QAbstractSocket::SocketResourceError,      errSocketResource},
        {QAbstractSocket::SocketTimeoutError,       errSocketTimedOut},
        {QAbstractSocket::DatagramTooLargeError,    errDataGramTooLong},
        {QAbstractSocket::NetworkError,             errNetWorkError},
        {QAbstractSocket::AddressInUseError,        errAddressInUse},
        {QAbstractSocket::TemporaryError,           errTemporary},
    };
    if (errorMap.contains(error))
        return errorMap[error];
    else
        return errUnidentified;
}

void OperationDescriptor::forwardConnected(QSharedPointer<OperationDescriptor> operation) {
    if (operation.data() == this)
        emit connected();
}

void OperationDescriptor::forwardDataSent(QSharedPointer<OperationDescriptor> operation) {
    if (operation.data() == this)
        emit dataSent();
}

void OperationDescriptor::forwardDataReceived(QSharedPointer<OperationDescriptor> operation, QByteArray data) {
    if (operation.data() == this)
        emit pendingDataReceived(data);
}

void OperationDescriptor::forwardConnectError(QSharedPointer<OperationDescriptor> operation, Error error) {
    if (operation.data() == this)
        emit connectError(error);
}

void OperationDescriptor::forwardSendReceiveError(QSharedPointer<OperationDescriptor> operation, OperationDescriptor::Error error) {
    if (operation.data() == this)
        emit sendReceiveError(error);
}

void OperationDescriptor::forwardFileProgress(QSharedPointer<OperationDescriptor> operation, quint64 bytesSent) {
    if (operation.data() == this)
        emit fileProgress(bytesSent);
}
