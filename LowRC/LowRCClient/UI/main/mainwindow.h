/// @file mainwindow.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "clientcore.h"
#include "operationdescriptor.h"
#include "basefiletransceiver.h"
#include "database.h"
#include "protocolconstant.h"

#include "titlebutton.h"
#include "clickablelabel.h"
#include "userinfodisplay.h"
#include "fileinfodisplay.h"
#include "chatpage.h"
#include "res.h"

#include <QMainWindow>
#include <QLabel>
#include <QMovie>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QTextStream>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    class FileIdentifier {
    private:
        static QQueue<quint16> pool;
        static quint16 maxid;

        quint16 id;
    public:
        /// @brief get id from pool
        FileIdentifier();
        /// @brief return id to pool
        ~FileIdentifier();

        /// @brief does not allow copy
        FileIdentifier(const FileIdentifier &) = delete;
        /// @brief does not allow copy
        FileIdentifier &operator =(const FileIdentifier &) = delete;

        /// @brief get a new identifier
        static QSharedPointer<FileIdentifier> getNewIdentifier();

        /// @brief get the id of the currend File identifier
        quint16 getId();
    };

    Ui::MainWindow *ui;

    /// @brief a record, serve for dragging function
    QPoint dragStartPosition;
    /// @brief a paramter used to prevent sudden move of the window after restoring
    bool windowRestored;
    /// @brief internal flag
    bool canMove;

    /// @brief the pool to store the operation (universal)
    QMap<OperationDescriptor *, QSharedPointer<OperationDescriptor>> operationPool;

    /// @brief where messages are stored in the first stage of sending a message
    /// {operation, {target, message}}
    QMap<OperationDescriptor *, QPair<QString, QByteArray>> sendMessageStage1;
    /// @brief where messages are stored in the second stage of sending a message
    QMap<OperationDescriptor *, QPair<QString, QByteArray>> sendMessageStage2;
    /// @brief where messages are stored in the caching stage of a message
    QMap<OperationDescriptor *, QPair<QString, QByteArray>> cacheMessageStage;

    /// @brief where the file path are stored in the first stage of sending a file
    /// the first stage is trying to connect to the peer, if it's successful, it will
    /// proceed to sendFileStage2.
    ///
    /// {operation, {target, filePath}}
    QMap<OperationDescriptor *, QPair<QString, QString>> sendFileStage1;

    /// @brief where the file path are stored in the second stage of sending a file
    /// the second stage is trying to send a message and get a response about the receiver's
    /// file listening port etc
    QMap<OperationDescriptor *, QPair<QString, QString>> sendFileStage2;
    /// @brief try sending a file stage
    QMap<OperationDescriptor *, QPair<QString, QString>> sendFileStage3;

    /// @brief where the file info are stored in the pending receive stage
    /// stage 1 is pending receive stage, user have to choose from receive or not
    /// {operation, incomeJson}
    QMap<OperationDescriptor *, QJsonObject> receiveFileStage1;
    /// @brief receive receiv file stage 2, the receiving process
    QMap<OperationDescriptor *, QPair<QSharedPointer<FileIdentifier>, FileInfoDisplay *> > receiveFileStage2;

    /// @brief the identifiler pool
    QQueue<quint16> fileIdentifierPool;

    /// @brief connect to user
    QString currentUser;
    /// @brief the current displaying chat page
    ChatPage *currentChatPage;

protected:
    // dragging function
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
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

    /// @brief try send a file to address:port
    void sendFile(QSharedPointer<OperationDescriptor> operation,
            QString fileName, quint16 identifier, QHostAddress address, quint16 port);

    /// @brief start to expecting an receive from address:port
    void createPendingReceive(QSharedPointer<OperationDescriptor> operation,
            QString fileName, quint64 fileSize, quint16 identifier,
            QHostAddress senderAddress, quint16 senderPort);
private slots:

    /// @brief the user's info is pressed
    void userInfoPressed(QString userEmail);

    /// @brief a user is connected
    void messageStage1Connected();
    /// @brief failed to connect to a user
    void messageStage1ConnectFailed(OperationDescriptor::Error error);
    /// @brief serve replied when trying to cache a message
    void messageCacheStageReplyed(QByteArray);
    /// @brief send stage 2 replyed
    void messageStage2DataSent();
    /// @brief faile to send message to peer
    void messageStage2FailedToSend(OperationDescriptor::Error);

    /// @{ send file slots
    /// @brief the user is connected
    void fileStage1Connected();
    /// @brief failed to connect to the target user
    void fileStage1ConnectFailed(OperationDescriptor::Error error);
    /// @brief get the peer reply of the message
    void fileStage2Replyed(QByteArray);
    /// @brief get the peer port
    void fileStage2SendReceiveError(OperationDescriptor::Error error);
    /// @brief the file is send
    void fileStage3Sent();
    /// @brief the file failed to send at last stage
    void fileStage3FailedToSend(OperationDescriptor::Error error);
    /// @brief the file transfer progress changed, update UI
    void fileProgressChanged(quint64);
    /// @}


    /// @brief the file receive is accepted (the accept button is pressed)
    void fileAccecpted(QSharedPointer<OperationDescriptor> opeartion);
    /// @brief the file receive is declined (the decline button is pressed)
    void fileDeclined(QSharedPointer<OperationDescriptor> operation);
    /// @brief the pending receive file is received
    void fileReceived();
    /// @brief the pending receive file's reception failed
    void fileReceiveFailed(OperationDescriptor::Error error);

    /// @brief handle unexpected error
    void unexpectedErrorOccurred(QHostAddress address, quint16 port, BaseConnector::UnexpectedError error);
    void unexpectedDataReceived(QHostAddress address, quint16 port, QByteArray data);
    void messageButtonPressed();
    void fileButtonPressed();
};

#endif // MAINWINDOW_H
