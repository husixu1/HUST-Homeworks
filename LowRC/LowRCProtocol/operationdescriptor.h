/// @file operationdescriptor.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef OPERATIONDESCRIPTOR_H
#define OPERATIONDESCRIPTOR_H

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QSharedPointer>
#include <QAbstractSocket>

class BaseConnector;

//> TODO: make a warpper of this class

/// @brief serve as a tracker of an operation
/// @brief also serve as a identifier of an operation
class OperationDescriptor : public QObject {
    Q_OBJECT
public:
    /// @brief the event to emit to outside
    enum Error {
        // connector error
        errNoError,
        errNotConnected,
        errHostConnected,
        errCannotConnectToHost,
        errPendingReceiveTimedOut,
        errInternal,

        // socket error
        errConnectionRefused,
        errRemoteHostClosed,
        errHostNotFound,
        errSocketResource,
        errSocketTimedOut,
        errDataGramTooLong,
        errNetWorkError,
        errAddressInUse,
        errTemporary,

        // file transfer error
        errFailedOpenFile,
        errUnidentified,
    };
    Q_ENUM(Error)

private:
    // infact, OperationDescriptor belongs to Base Connector
    // but nested class are not allowed to have Qt signals and slots
    friend class BaseConnector;

    /// @brief the global unique id
    quint64 id;

    /// @{
    /// @brief get opreationID from this pool, and return id when operation done
    /// when operationID is empty while require for an id, increase the maxOpeartionID;
    /// when the very maxOperationID is returned to the pool,
    /// decrease the maxOperationID (till 0)
    /// @note for the compatibility of the LowRCProxy, id starts from 1
    static QQueue<quint64> idPool;
    static quint64 idNum;
    /// @}

    /// @brief constructor, generate a unique descriptor, id starts from 1
    OperationDescriptor();

public:
    /// @brief a helper funtion to get a new operation Discriptor's smart pointer
    /// @note emitting message and use QSharedPointer class as a parameter cannot
    /// add reference count to the pointer, and it'll be released if it's not stored explicitly.
    static QSharedPointer<OperationDescriptor> getNewOperationDescriptor();

    /// @{
    /// @brief does not allow copy, use reference/pointer instead
    OperationDescriptor(const OperationDescriptor &) = delete;
    OperationDescriptor &operator =(const OperationDescriptor &) = delete;
    /// @}

    /// @brief de-construct, return the descriptor to the pool
    /// @note deletion is done by user
    ~OperationDescriptor();

    /// @brief get the id of the descriptor
    quint64 getId();

    /// @brief compair if they'rE the same descriptor
    bool operator ==(const OperationDescriptor &);
    /// @brief compair if they're not the same descriptor
    bool operator !=(const OperationDescriptor &);

    /// @brief translate socket error to native error
    static Error translateSocketError(QAbstractSocket::SocketError error);

public slots:
    /// @{
    /// @brief forwarding the signals from BaseConnector
    void forwardConnected(QSharedPointer<OperationDescriptor> operation);
    void forwardDataSent(QSharedPointer<OperationDescriptor> operation);
    void forwardDataReceived(QSharedPointer<OperationDescriptor> operation, QByteArray data);
    void forwardConnectError(QSharedPointer<OperationDescriptor> operation, OperationDescriptor::Error error);
    void forwardSendReceiveError(QSharedPointer<OperationDescriptor> operation, OperationDescriptor::Error error);
    void forwardFileProgress(QSharedPointer<OperationDescriptor> operation, quint64 bytesSent);
    /// @}

signals:
    /// @brief forwarding the socket's connected signal
    void connected();
    /// @brief the data is successfully sent
    void dataSent();
    /// @brief the pending data has been received
    void pendingDataReceived(QByteArray data);

    /// @brief error occurred when trying to connect
    void connectError(OperationDescriptor::Error error);
    /// @brief error occurred when trying to send/receive data
    void sendReceiveError(OperationDescriptor::Error error);

    /// @brief signal emitted while file transfer percentage changed
    void fileProgress(quint64 bytesSent);
};


#endif // OPERATIONDESCRIPTOR_H
