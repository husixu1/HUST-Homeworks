/// @file mainwindow.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logindialog.h"
#include <QSizeGrip>
namespace PC = ProtocolConstant;

/// @brief internal key used by the file transfer process
static const QString keySenderAddress = "__senderAddress";
/// @brief internal key used by the file transfer process
static const QString keySenderPort = "__senderPort";
/// @brief internal key used by the file transfer process
static const QString keySenderReqNum = "__senderReqNum";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //> add to database
    const QList<const DataBase::UserInfo *> userList = DataBase::getInstance().getUserList();
    for (auto user : userList) {
        UserInfoDisplay *userInfoDisplay = new UserInfoDisplay;
        connect(userInfoDisplay, SIGNAL(userInfoPressed(QString)),
                this, SLOT(userInfoPressed(QString)));

        userInfoDisplay->setUserEmail(user->email);
        userInfoDisplay->setUserName(user->username);
        if (!user->messageLog.empty())
            userInfoDisplay->setMessage(user->messageLog.last().message);
        ui->userPanel->layout()->addWidget(userInfoDisplay);
        userInfoDisplay->show();
    }

    //> connect signals to connector
    connect(this, SIGNAL(tryConnect(
                            QSharedPointer<OperationDescriptor>, QHostAddress, quint16)),
            ClientCore::getInstance().getConnector(), SLOT(tryConnect(
                            QSharedPointer<OperationDescriptor>, QHostAddress, quint16)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress,
                            quint16, quint64, BaseConnector::DataType, QByteArray)),
            ClientCore::getInstance().getConnector(), SLOT(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress,
                            quint16, quint64, BaseConnector::DataType, QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(disconnectHost(QHostAddress, quint16)),
            ClientCore::getInstance().getConnector(), SLOT(disconnectHost(QHostAddress, quint16)),
            Qt::QueuedConnection);
    //> connect signals to file transceiver
    connect(this, SIGNAL(sendFile(
                            QSharedPointer<OperationDescriptor>, QString, quint16, QHostAddress, quint16)),
            ClientCore::getInstance().getFileTransceiver(), SLOT(sendFile(
                            QSharedPointer<OperationDescriptor>, QString, quint16, QHostAddress, quint16)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(createPendingReceive(
                            QSharedPointer<OperationDescriptor>, QString,
                            quint64, quint16, QHostAddress, quint16)),
            ClientCore::getInstance().getFileTransceiver(), SLOT(createPendingReceive(
                            QSharedPointer<OperationDescriptor>, QString,
                            quint64, quint16, QHostAddress, quint16)),
            Qt::QueuedConnection);

    //> connect unexpected signal/error
    connect(ClientCore::getInstance().getConnector(), SIGNAL(unexpectedError(
                            QHostAddress, quint16, BaseConnector::UnexpectedError)),
            this, SLOT(unexpectedErrorOccurred(
                            QHostAddress, quint16, BaseConnector::UnexpectedError)),
            Qt::QueuedConnection);
    connect(ClientCore::getInstance().getConnector(), SIGNAL(unexpectedDataReceived(
                            QHostAddress, quint16, QByteArray)),
            this, SLOT(unexpectedDataReceived(
                            QHostAddress, quint16, QByteArray)),
            Qt::QueuedConnection);

    /// core ///////////
    ///////////// ui ///

    //> make the window boderless
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //> set window icon
    setWindowIcon(QIcon(resSysTrayIcon));

    //> add the title buttons and bind the events
    TitleButton *titleButton = new TitleButton(this);
    titleButton->move(QPoint(this->size().width() - titleButton->size().width(), 0));
    titleButton->show();
    //> connect close button signal
    connect(titleButton->getCloseButton(), SIGNAL(clicked()), this, SLOT(close()));

    //> connect two send button signal
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(messageButtonPressed()));
    connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(fileButtonPressed()));

    //> send/file buttons disabled by default
    ui->sendButton->setEnabled(false);
    ui->fileButton->setEnabled(false);
    ui->textPanel->setEnabled(false);

    //> hide file widget
    ui->fileWidget->hide();

    currentChatPage = nullptr;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::userInfoPressed(QString userEmail) {

    //> update current updated user
    currentUser = userEmail;

    //> replace chat panel
    if (currentChatPage) {
        delete currentChatPage;
        currentChatPage = nullptr;
    }
    currentChatPage = new ChatPage(ui->messageArea);
    ui->messageArea->setWidget(currentChatPage);

    //> load history messages from database
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(userEmail);
    if (userInfo) {
        for (auto message : userInfo->messageLog) {
            if (message.isFromSelf)
                currentChatPage->addMessageForMe(message.message);
            else
                currentChatPage->addMessageForPeer(message.message);
        }
    }

    //> enable send/file buttons
    ui->sendButton->setEnabled(true);
    ui->fileButton->setEnabled(true);
    ui->textPanel->setEnabled(true);

    currentChatPage->show();
}

void MainWindow::unexpectedErrorOccurred(
        QHostAddress address, quint16 port, BaseConnector::UnexpectedError error) {

    //> if server disconnected
    if (address == ClientCore::getInstance().serverAddress() &&
            port == ClientCore::getInstance().serverPort()) {
        //TODO: fall to before-login status
        LoginDialog *fallback = new LoginDialog(nullptr, true);
        fallback->show();
        close();
    }
}

void MainWindow::unexpectedDataReceived(QHostAddress address, quint16 port, QByteArray rawData) {
    //> resolve data
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;
    BaseConnector::resolveData(rawData, type, reqNum, data);

    QJsonObject incomeJson = QJsonDocument::fromBinaryData(data).object();

    qDebug().noquote() << "Unexpected data: addr: " << address.toString() << "port: " << port << endl
            << QJsonDocument::fromBinaryData(data).toJson();

    //> deal with unexpected incoming error
    switch (type) {
        case BaseConnector::typeMessageP2P: {
            QString peerEmail = incomeJson[PC::keyMap[PC::keyEmail]].toString();
            const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(peerEmail);
            //> if the user is found
            if (userInfo) {
                DataBase::getInstance().addMessageToLog(
                        peerEmail, incomeJson[PC::keyMap[PC::keyMessage]].toString(), true);
                //> add to display
                if (currentUser == peerEmail)
                    currentChatPage->addMessageForPeer(incomeJson[PC::keyMap[PC::keyMessage]].toString());
            }
            break;
        }
        case BaseConnector::typeUserBroadcast: {
            qDebug() << "Data is user Broadcast";
            QString peerEmail = incomeJson[PC::keyMap[PC::keyEmail]].toString();
            bool isOnline = incomeJson[PC::keyMap[PC::keyIsOnline]].toBool();
            QHostAddress address(incomeJson[PC::keyMap[PC::keyAddress]].toString());
            quint16 port = static_cast<quint16>(incomeJson[PC::keyMap[PC::keyPort]].toInt());
            //> update user's online status
            DataBase::getInstance().updateOnlineStatus(peerEmail, isOnline, address, port);
            //> TODO: update UI
            break;
        }
        case BaseConnector::typeFile1P2P: {

            //> insert operation into pool
            QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
            operationPool.insert(operation.data(), operation);
            //> add sender's message address and port to the map for later use
            incomeJson.insert(keySenderAddress, address.toString());
            incomeJson.insert(keySenderPort, port);
            incomeJson.insert(keySenderReqNum, QString::number(reqNum));
            receiveFileStage1.insert(operation.data(), incomeJson);

            //> show a pending receive
            ui->fileWidget->show();
            //>> create a fileInfo
            FileInfoDisplay *fileInfo = new FileInfoDisplay;
            QString fullName = incomeJson[PC::keyMap[PC::keyFileName]].toString(), fileName;
            if (fullName.contains('/'))
                fileName = fullName.right(fullName.size() - fullName.lastIndexOf('/') - 1);
            else
                fileName = fullName;
            fileInfo->setFileName(fileName);
            fileInfo->setStatus(FileInfoDisplay::statusReceiveInitial);
            fileInfo->setCorrespondingOperation(operation);

            //>> connect signals
            connect(fileInfo, SIGNAL(fileAccecpted(QSharedPointer<OperationDescriptor>)),
                    this, SLOT(fileAccecpted(QSharedPointer<OperationDescriptor>)));

            ui->filePanel->layout()->addWidget(fileInfo);
            fileInfo->show();
            break;
        }
        default:
            qDebug() << "not recognised data";
            break;
    }
}

/////////// Message Transfer ////////////
void MainWindow::messageButtonPressed() {
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(currentUser);

    //> if trying to send a message to an unexsist user
    if (!userInfo)
        return;

    if (ui->textPanel->toPlainText().size() == 0) {
        ui->textPanel->setPlaceholderText("/* you have to write something to send */");
        return;
    }

    //> construct message
    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyEmail], ClientCore::getInstance().getMyEmail());
    sendJson.insert(PC::keyMap[PC::keyTargetEmail], currentUser);
    sendJson.insert(PC::keyMap[PC::keyMessage], ui->textPanel->toPlainText());
    QByteArray message = QJsonDocument(sendJson).toBinaryData();

    //> add the message to the pool
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    operationPool.insert(operation.data(), operation);

    // judge if user is online
    if (userInfo->isOnline) {
        //> if user is online, try connect to that user
        sendMessageStage1.insert(operation.data(), {currentUser, message});

        //> try connecting to the user
        connect(operation.data(), SIGNAL(connected()),
                this, SLOT(messageStage1Connected()));
        connect(operation.data(), SIGNAL(connectError(OperationDescriptor::Error)),
                this, SLOT(messageStage1ConnectFailed(OperationDescriptor::Error)));
        emit tryConnect(operation, userInfo->address, userInfo->port);
    } else {
        //> if user is offline, try cache the message to the server
        cacheMessageStage.insert(operation.data(), {currentUser, message});

        connect(operation.data(), SIGNAL(pendingDataReceived(QByteArray)),
                this, SLOT(messageCacheStageReplyed(QByteArray)));
        connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
                this, SLOT(messageStage2FailedToSend(OperationDescriptor::Error)));

        //> try send to server
        emit trySendData(operation, ClientCore::getInstance().serverAddress(),
                ClientCore::getInstance().serverPort(), operation->getId(),
                BaseConnector::typeMessageRequest, message);
    }
    //> TODO: add message to the panel, setting not send status

    ui->textPanel->clear();
}

void MainWindow::messageStage1Connected() {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(sendMessageStage1[operation].first);
    QByteArray message = sendMessageStage1[operation].second;

    //> move to stage 2
    sendMessageStage2.insert(operation, sendMessageStage1[operation]);
    sendMessageStage1.remove(operation);
    operation->disconnect();

    connect(operation, SIGNAL(dataSent()),
            this, SLOT(messageStage2DataSent()));
    connect(operation, SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(messageStage2FailedToSend(OperationDescriptor::Error)));

    emit trySendData(operationPool[operation], userInfo->address,
            userInfo->port, operation->getId(),
            BaseConnector::typeMessageP2P, message);
}

void MainWindow::messageStage1ConnectFailed(OperationDescriptor::Error) {
    // if connect failed, send message to server
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());

    //> if user is offline, try cache the message to the server
    //>> move to cachestage
    QByteArray message = sendMessageStage1[operation].second;
    cacheMessageStage.insert(operation,
    {sendMessageStage1[operation].first, sendMessageStage1[operation].second});
    sendMessageStage1.remove(operation);

    operation->disconnect();
    connect(operation, SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(messageCacheStageReplyed(QByteArray)));
    connect(operation, SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SIGNAL(messageStage2FailedToSend(OperationDescriptor::Error)));

    //> try send to server
    emit trySendData(operationPool[operation], ClientCore::getInstance().serverAddress(),
            ClientCore::getInstance().serverPort(), operation->getId(),
            BaseConnector::typeMessageRequest, message);
}

void MainWindow::messageCacheStageReplyed(QByteArray rawData) {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;
    BaseConnector::resolveData(rawData, type, reqNum, data);

    QJsonObject replyJson = QJsonDocument::fromBinaryData(data).object();

    QString message = QJsonDocument::fromBinaryData(cacheMessageStage[operation].second)
            .object()[PC::keyMap[PC::keyMessage]].toString();

    if (replyJson[PC::keyMap[PC::keyReply]] == PC::replyValueMap[PC::replyOK]) {
        DataBase::getInstance().addMessageToLog(
                cacheMessageStage[operation].first,
                message, false);
        //> message send successfully, UI response
        if (currentUser == cacheMessageStage[operation].first) {
            currentChatPage->addMessageForMe(message
            );
        }
    } else {
        //> TODO: failed, UI response
    }

    cacheMessageStage.remove(operation);
    operationPool.remove(operation);
}

void MainWindow::messageStage2DataSent() {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());

    QString message = QJsonDocument::fromBinaryData(sendMessageStage2[operation].second)
            .object()[PC::keyMap[PC::keyMessage]].toString();

    //> add message to database
    DataBase::getInstance().addMessageToLog(
            sendMessageStage2[operation].first,
            message, false);

    //> UI response
    if (currentUser == sendMessageStage2[operation].first) {
        currentChatPage->addMessageForMe(message);
    }

    sendMessageStage2.remove(operation);
    operationPool.remove(operation);
}

void MainWindow::messageStage2FailedToSend(OperationDescriptor::Error) {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());

    //TODO: failed to send message (UI)

    sendMessageStage2.remove(operation);
    cacheMessageStage.remove(operation);
    operationPool.remove(operation);
}

/////////// File Transfer Send ////////////
void MainWindow::fileButtonPressed() {
    //> open a file window to select a file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose a file"));
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(currentUser);

    //> init operations
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    connect(operation.data(), SIGNAL(connected()),
            this, SLOT(fileStage1Connected()));
    connect(operation.data(), SIGNAL(connectError(OperationDescriptor::Error)),
            this, SLOT(fileStage1ConnectFailed(OperationDescriptor::Error)));

    //> insert operation to maps, enter stage 1
    operationPool.insert(operation.data(), operation);
    sendFileStage1.insert(operation.data(), {currentUser, filePath});

    if (userInfo && userInfo->isOnline) {
        //> if user is online, try to connect to peer
        sendFileStage1.insert(operation.data(), {currentUser, filePath});
        emit tryConnect(operation, userInfo->address, userInfo->port);
    } else {
        //TODO: if user is offline (UI)
        qDebug() << "user is offline, caching file to server";
    }
}

void MainWindow::fileStage1Connected() {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());

    //> continue to stage 2
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(sendFileStage1[operation].first);
    QString fileName = sendFileStage1[operation].second;
    sendFileStage2.insert(operation, sendFileStage1[operation]);
    sendFileStage1.remove(operation);

    //> construct json
    QFile file(fileName);
    if (!file.exists()) {
        // TODO: file do not exist
    }
    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyFileName], fileName);
    sendJson.insert(PC::keyMap[PC::keyFileSize], QString::number(file.size()));
    sendJson.insert(PC::keyMap[PC::keyPort], ClientCore::getInstance().filePort());

    //> re-connect signals
    operation->disconnect();
    connect(operation, SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(fileStage2Replyed(QByteArray)));
    connect(operation, SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(fileStage2SendReceiveError(OperationDescriptor::Error)));

    if (userInfo->isOnline) {
        //> if user is online
        emit trySendData(operationPool[operation], userInfo->address, userInfo->port,
                operation->getId(), BaseConnector::typeFile1P2P, QJsonDocument(sendJson).toBinaryData());
        qDebug() << "file request send, id = " << operation->getId() << "operation is " << operationPool[operation].isNull();
    } else {
        // TODO: if user is offline, cache file to server
    }
}

void MainWindow::fileStage1ConnectFailed(OperationDescriptor::Error error) {
    //> TODO: failed to connect to peer, caching file to server
}

void MainWindow::fileStage2Replyed(QByteArray rawData) {

    //> resolve data
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;
    BaseConnector::resolveData(rawData, type, reqNum, data);
    QJsonObject incomeJson = QJsonDocument::fromBinaryData(data).object();
    qDebug().noquote() << "file Stage 2 replyed, data is: " << QJsonDocument(incomeJson).toJson();

    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    //> move forward to stage 3
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(sendFileStage2[operation].first);
    QString fileName = sendFileStage2[operation].second;
    sendFileStage3.insert(operation, sendFileStage2[operation]);
    sendFileStage2.remove(operation);

    quint16 identifier = static_cast<quint16>(incomeJson[PC::keyMap[PC::keyIdentifier]].toInt());
    quint16 peerFilePort = static_cast<quint16>(incomeJson[PC::keyMap[PC::keyPort]].toInt());

    //> re-connect signals
    operation->disconnect();
    connect(operation, SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(fileStage3FailedToSend(OperationDescriptor::Error)));

    // TODO: move the construction of the UI to fileButtonPressed
    //> init UI (fileInfoDisplay)
    QString shortFileName;
    FileInfoDisplay *fileInfo = new FileInfoDisplay;
    if (fileName.contains('/'))
        shortFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    else
        shortFileName = fileName;
    fileInfo->setFileName(shortFileName);
    fileInfo->setTotalBytes(QFile(fileName).size());
    fileInfo->setStatus(FileInfoDisplay::statusSendBegan);
    //>> show fileInfoDisplay
    ui->filePanel->layout()->addWidget(fileInfo);
    fileInfo->show();
    ui->fileWidget->show();

    //> TODO: change lambda to fileStage3Sent
    connect(operation, &OperationDescriptor::dataSent, [ = ]() {
        fileInfo->setStatus(FileInfoDisplay::statusSenFinished);
        sendFileStage3.remove(operation);
        operationPool.remove(operation);
    });
    //>> connect update progress signals
    connect(operation, &OperationDescriptor::fileProgress, [ = ](quint64 bytesSent) {
        fileInfo->setProgress(bytesSent);
    });

    // start sending
    if (userInfo && userInfo->isOnline) {
        //> if user is online, send file
        emit sendFile(operationPool[operation], fileName, identifier, userInfo->address, peerFilePort);
        qDebug() << "file send started";
    } else {
        //> TODO: cache file to server
        qDebug() << "user offline";
    }
}

void MainWindow::fileStage2SendReceiveError(OperationDescriptor::Error error) {
    //TODO:
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    sendFileStage2.remove(operation);
    operationPool.remove(operation);
    qDebug() << error;
}

void MainWindow::fileStage3Sent() {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    //TODO: ui
    // TODO: remove descriptor from map
    sendFileStage3.remove(operation);
    operationPool.remove(operation);
}

void MainWindow::fileStage3FailedToSend(OperationDescriptor::Error error) {
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    sendFileStage3.remove(operation);
    //TODO: ui
    //TODO: delete file
    operationPool.remove(operation);
    qDebug() << "failed to send file" << error;
}

void MainWindow::fileProgressChanged(quint64) {
    //TODO: ui
}

/////////// File Transfer Receive ////////////
void MainWindow::fileAccecpted(QSharedPointer<OperationDescriptor> operation) {
    //> init file display
    QJsonObject incomeJson = receiveFileStage1[operation.data()];
    FileInfoDisplay *fileDisplay = qobject_cast<FileInfoDisplay *>(sender());
    fileDisplay->setTotalBytes(static_cast<quint64>(incomeJson[PC::keyMap[PC::keyFileSize]].toString().toULongLong()));

    //> create new operation and identifier
    QSharedPointer<OperationDescriptor> fileReceiveOperation =
            OperationDescriptor::getNewOperationDescriptor();
    QSharedPointer<FileIdentifier> fileIdentifier = FileIdentifier::getNewIdentifier();
    operationPool.insert(fileReceiveOperation.data(), fileReceiveOperation);
    receiveFileStage2.insert(fileReceiveOperation.data(), {fileIdentifier, fileDisplay});

    //> reply a typeFile2P2P message
    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyPort], ClientCore::getInstance().filePort());
    sendJson.insert(PC::keyMap[PC::keyIdentifier], fileIdentifier->getId());
    sendJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);

    connect(fileReceiveOperation.data(), SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(fileReceived()));
    connect(fileReceiveOperation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(fileReceiveFailed(OperationDescriptor::Error)));
    connect(fileReceiveOperation.data(), &OperationDescriptor::fileProgress, [ = ](quint64 progress) {
        fileDisplay->setProgress(progress);
    });

    //> ask user to select a place to store the file (blocking)
    QString storePath = QFileDialog::getSaveFileName(this, QString(), incomeJson[PC::keyMap[PC::keyFileName]].toString());

    //> create a pending receive and start to wait for receive
    emit createPendingReceive(fileReceiveOperation, storePath,
            incomeJson[PC::keyMap[PC::keyFileSize]].toString().toULongLong(), fileIdentifier->getId(),
            QHostAddress(incomeJson[keySenderAddress].toString()),
            incomeJson[PC::keyMap[PC::keyPort]].toInt());

    //> connect signals
    //> try to send data, tell the peer to send the file
    emit trySendData(fileReceiveOperation,
            QHostAddress(incomeJson[keySenderAddress].toString()),
            static_cast<quint16>(incomeJson[keySenderPort].toInt()),
            incomeJson[keySenderReqNum].toString().toULong(), BaseConnector::typeFile2P2P,
            QJsonDocument(sendJson).toBinaryData());
    qDebug() << "file reply send, peer addr: " << incomeJson[keySenderAddress].toString()
            << "port: " << incomeJson[keySenderPort].toInt() << endl
            << "reqnum = " << incomeJson[keySenderReqNum].toString().toULong();

    //> change the status of the receiveing span
    fileDisplay->setStatus(FileInfoDisplay::statusReceiveBegan);

    //> remove the old operation
    receiveFileStage1.remove(operation.data());
    operationPool.remove(operation.data());
}

void MainWindow::fileDeclined(QSharedPointer<OperationDescriptor> operation) {
    //TODO: emit declined
    qDebug() << "file declined";
}

void MainWindow::fileReceived() {
    //> set the display
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    FileInfoDisplay *fileDisplay = receiveFileStage2[operation].second;
    fileDisplay->setStatus(FileInfoDisplay::statusReceiveFinished);

    //> remove from operation pool
    receiveFileStage2.remove(operation);
    operationPool.remove(operation);

    qDebug() << "file received";
}

void MainWindow::fileReceiveFailed(OperationDescriptor::Error error) {
    qDebug() << "failed To receieve file, error " << error;
    // TODO: UI
}


////////// UI events ////////////
// drag-to-move functions
void MainWindow::mousePressEvent(QMouseEvent *event) {
    event->accept();

    if (event->pos().y() > ui->titleBar->height()) {
        canMove = false;
        return;
    }

    canMove = true;
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    if (canMove) {
        // if window just restored, don't move
        if (windowRestored) {
            windowRestored = false;
            return;
        }
        if (event->buttons() & Qt::LeftButton)
            move(event->globalPos() - dragStartPosition);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *) {
    canMove = false;
}


///////////// File identifier ///////////////


QQueue<quint16> MainWindow::FileIdentifier::pool;
quint16 MainWindow::FileIdentifier::maxid = 0;

MainWindow::FileIdentifier::FileIdentifier() {
    if (pool.isEmpty()) {
        ++maxid;
        id = maxid;
    } else {
        id = pool.front();
        pool.pop_front();
    }
}

MainWindow::FileIdentifier::~FileIdentifier() {
    if (maxid == id) {
        --maxid;
    } else {
        pool.push_back(id);
    }
}

QSharedPointer<MainWindow::FileIdentifier> MainWindow::FileIdentifier::getNewIdentifier() {
    if (maxid == UINT16_MAX) {
        return QSharedPointer<FileIdentifier>(nullptr);
    } else {
        return QSharedPointer<FileIdentifier>(new FileIdentifier);
    }
}

quint16 MainWindow::FileIdentifier::getId() {
    return id;
}
