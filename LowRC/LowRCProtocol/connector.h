/// @file connector.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "baseconnector.h"
#include "operationdescriptor.h"

#include <QObject>
#include <QThread>
#include <QHostAddress>
#include <QSharedPointer>

Q_DECLARE_METATYPE(QHostAddress)
Q_DECLARE_METATYPE(QSharedPointer<OperationDescriptor>)
Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(OperationDescriptor::Error)
Q_DECLARE_METATYPE(BaseConnector::DataType)

/// @brief a wrapper to baseConnector
/// this class is used to initialize, control and de-initialize BaseConnector,
/// it creates a new QThread and run BaseConnector on it, and forward some of
/// BaseConnector's sigals
class Connector : public QObject {
    Q_OBJECT
private:

    /// @brief singleton assurance
    Connector() = default;

    /// @brief the thread that BaseConnector instance is running on
    QThread thread;

public:
    /// @brief singleton assurance
    Connector(const Connector &) = delete;
    /// @brief singleton assurance
    Connector &operator =(const Connector &) = delete;
    /// @brief de-constructor, stop the thread and delete the connector
    ~Connector();

    /// @brief initialize the BaseConnnector
    void init();

signals:
    /// @brief error when trying to listen to the port
    /// @note when listenError occurred, this connector is no longer useful. user should delete it
    void listenError();

    /// @brief the connector has started listening, give the host and port
    void listenStarted(QHostAddress, quint16);

    /// @brief host connected successfully
    /// @note this error should be received from operation descriptor
    void connected(QSharedPointer<OperationDescriptor>);
    /// @brief error when trying to connecte
    /// @note this error should be received from operation descriptor
    void connectError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error);

    /// @brief data sent successfully
    /// @note this error should be received from operation descriptor
    void dataSent(QSharedPointer<OperationDescriptor>);
    /// @brief expected data received successfully
    /// @note this error should be received from operation descripto
    void dataReceived(QSharedPointer<OperationDescriptor>);
    /// @brief error when trying to send/receive
    /// @note this error should be received from operation descriptor
    void sendReceiveError(QSharedPointer<OperationDescriptor>, OperationDescriptor::Error);

    /// @brief indicates that an unexpected error has occurred
    void unexpectedError(QHostAddress, quint16, UnexpectedError error);

    /// @brief send the data to the processor
    void processData(QByteArray data);

public slots:

    /// @brief start listening on random addr/port
    void startListen();

    /// @brief start listening on specific port
    void startListen(quint16 port);

    /// @brief connect to host
    void tryConnect(QSharedPointer<OperationDescriptor> operation,
            QHostAddress address, quint16 port);

    /// @brief send data to designated destination
    void trySendData(QSharedPointer<OperationDescriptor> operation,
            QHostAddress address, quint16 port, BaseConnector::DataType type, QByteArray data);

    /// @brief disconnect from host ip:port
    void disconnectHost(QHostAddress address, quint16 port);
};

#endif // CONNECTOR_H
