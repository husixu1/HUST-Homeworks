/// @file dataprocessor.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "baseconnector.h"
#include "operationdescriptor.h"
#include "database.h"
#include "protocolconstant.h"

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QSharedPointer>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

// TODO: switch to singleton mode

class DataProcessor : public QObject {
    Q_OBJECT
private:

    /// @brief used to store the OperationDescriptor to prevent it from auto-release
    QMap<OperationDescriptor *, QSharedPointer<OperationDescriptor>> operationPool;

    /// @brief the pending operations man
    /// this map stores OperationDescriptor and it's related request type and data to process.
    /// data are processed differently according to datatype
    QMap<OperationDescriptor *, QPair<BaseConnector::DataType, QJsonObject> > operationMap;

    /// @brief map that stores validate key
    /// A validate key is a random generated number that used to identify
    /// the connection between two respond, that is, the server is expecting a
    /// pending request after a former respond with key "validate" whoes
    /// value is in this validateMap. e.g. BaseConnector::typePassword2Request and
    /// BaseConnector::typePassword3Request.
    /// the key  is email + validateNum and value is the timeout timer
    QMap<QString, QSharedPointer<QTimer>> validateMap;

    /// @brief the reverse-searching map using timer as key and validation number as vaule
    /// @see validateMap
    QMap<QTimer *, QString> validateTimerMap;

    /// @brief 5 minute to respond
    static const int validateTimeout = 3000000;

    /// @brief a key for the LonginRequest event
    /// this key is used to temporarily store the address of the connected client
    /// for further process
    static const QString keyConnectAddress;

    /// @brief a key for the LonginRequest event
    /// this key is used to temporarily store the port of the connected client
    /// for further process
    static const QString keyConnectPort;
public:
    /// @brief constructor
    DataProcessor() = default;

    /// @brief does not allow copy
    DataProcessor(const DataProcessor &) = delete;
    /// @brief does not allow copy
    DataProcessor &operator =(const DataProcessor &) = delete;

    /// @brief de-constructor
    ~DataProcessor() = default;

signals:
    /// @brief send the data to the client
    void sendData(QSharedPointer<OperationDescriptor> operation,
            QHostAddress address, quint16 port,
            quint64 reqNum, BaseConnector::DataType type, QByteArray data);

private slots:
    /// @brief when failed to send the data
    void failedToSend(OperationDescriptor::Error);

    /// @brief the data is successfully send
    void dataSent();

public slots:
    /// @brief process the received data
    void processData(QHostAddress address, quint16 port, QByteArray data);

    /// @brief process unexpected error send by baseconnector
    void unexpectedError(QHostAddress address, quint16 port, BaseConnector::UnexpectedError error);
};

#endif // DATAPROCESSOR_H
