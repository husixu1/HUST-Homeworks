/// @file basefiletransfer.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef BASEFILETRANSFER_H
#define BASEFILETRANSFER_H

#include "operationdescriptor.h"

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QSharedPointer>
#include <QThread>
#include <QFile>
#include <QMap>
#include <QBitArray>
#include <QTimer>
#include <QNetworkDatagram>
#include <QtDebug>

//> TODO: make a warpper of this class

/// @brief a file transferer using UDP socket.
/// @note this object should be run on another thread other than the UI thread
class BaseFileTransceiver : public QObject {
    Q_OBJECT
private:
    /// @brief a struct use to identify the sending file
    struct FileInfo {
        /// @brief the file to send
        QFile file;
        /// @brief the identifier number of the file
        quint16 identifier;
        /// @brief the address to send to
        QHostAddress address;
        /// @brief the port to send to
        quint16 port;
        /// @brief a map recording which part of the file is send successfully
        /// can transfer file up to 2T
        QBitArray progress;
        /// @brief a temporary pointer pointed to the current sending/receiving
        int pos;

        /// @brief the timeout timer
        QTimer timeoutTimer;
    };

    /// @brief the file segementation size
    const static quint64 segmentSize = 40000;
    const static int headerSize = 11;

    /// @brief set timeout to 10 sec
    const static quint64 timeoutInterval = 10000;

    /// @brief the socket used to transfer file
    QUdpSocket *socket;

    /// @brief the files in sending progresss
    QMap<QSharedPointer<OperationDescriptor>, QSharedPointer<FileInfo>> pendingSendMap;
    /// @brief unique marker use to quickly find the file from datagram
    /// Maps senderAddress+senderPort+identifier to operation.
    /// Use operation to find fileinfo in pendingSendMap
    QMap<QString, QSharedPointer<OperationDescriptor>> sendIdentifierMap;

    QMap<const QTimer *, QSharedPointer<OperationDescriptor> > sendTimerMap;

    /// @brief the files in receive progress
    QMap<QSharedPointer<OperationDescriptor>, QSharedPointer<FileInfo>> pendingReceiveMap;
    /// @brief unique marker use to quickly find the file from datagram
    /// Maps senderAddress+senderPort+identifier to operation.
    /// Use operation to find fileinfo in pendingReceiveMap
    QMap<QString, QSharedPointer<OperationDescriptor>> receiveIdentifierMap;

    QMap<const QTimer *, QSharedPointer<OperationDescriptor>> receiveTimerMap;

    void insertToSendMap(QSharedPointer<OperationDescriptor> operation, QSharedPointer<FileInfo> info);
    void removeFromSendMap(QSharedPointer<OperationDescriptor> operation);

    void insertToReceiveMap(QSharedPointer<OperationDescriptor> operation, QSharedPointer<FileInfo> info);
    void removeFromReceiveMap(QSharedPointer<OperationDescriptor> operation);

    /// @brief the timer used to check and send every iteration of the event loop
    QTimer *sendTimer;

    /// @brief send interval
    int sentTimerTimeoutInterval = 1;

    /// @brief a util function used to caltulate crc32
    uint32_t crc32(const char *buf, quint64 size);
public:

    /// @brief does not allow copy
    BaseFileTransceiver(const BaseFileTransceiver &) = delete;
    /// @brief does not allow copy
    BaseFileTransceiver &operator =(const BaseFileTransceiver &) = delete;

    /// @brief constructor
    BaseFileTransceiver() = default;

    /// @brief de-constructor
    ~BaseFileTransceiver();

    /// @brief init the file transferor
    void init();

signals:
    /// @brief forward the error occurred in the file send process
    void forwardSendReceiveError(QSharedPointer<OperationDescriptor> operation,
            OperationDescriptor::Error error);

    /// @brief forward the file sent signal
    void forwardFileSent(QSharedPointer<OperationDescriptor> operation);

    /// @brief the file received signal
    void forwardFileReceived(QSharedPointer<OperationDescriptor> operation, QByteArray filename);

    /// @brief forward the file send progress signal
    void forwardFileProgress(QSharedPointer<OperationDescriptor> operation, quint64 bytesSent);

    /// @brief if the udp socket is failed to bind to a port
    /// @brief use queued connection for this signal
    void failedToListen();
    /// @brief the file transferer is successfully initialized, pass the listening port of the socket
    /// @brief use queued connection for this signal
    void listenStarted(quint16 filePort);

    /// @brief an unexpected error happed in the socket
    /// @brief use queued connection for this signal
    void unexpectedError(QAbstractSocket::SocketError);

private slots:
    /// @brief check if there's file to send
    void sendTimerTimedOut();

    /// @brief process incoming data
    void processData();

    /// @brief no data received in the sending process
    void pendingSendTimedOut();

    /// @brief no data received in the receive process
    void pendingReceiveTimedOut();

    /// @brief initialize the file transferor
    void initialize();

public slots:
    /// @brief send the file to the address:port
    /// if not opened, errFileNotOpened wil be emitted through
    /// OperationDescriptor's sendReceiveError signal, file progress will
    /// be emitted in OperationDescriptor's fileProgressChanged signal
    /// @brief the indentifier must be different for each file, this should be guranteed by caller.
    void sendFile(QSharedPointer<OperationDescriptor> operation,
            QString fileName, quint16 identifier, QHostAddress address, quint16 port);

    /// @brief start to expecting an receive from address:port
    void createPendingReceive(QSharedPointer<OperationDescriptor> operation,
            QString fileName, quint64 fileSize, quint16 identifier,
            QHostAddress senderAddress, quint16 senderPort);
};

#endif // BASEFILETRANSFER_H
