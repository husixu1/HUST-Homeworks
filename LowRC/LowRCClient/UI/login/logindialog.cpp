#include"logindialog.h"
#include"ui_logindialog.h"

namespace PC = ProtocolConstant;

// constructoin and deconstruction functions
LoginDialog::LoginDialog(QWidget *parent, bool isFallback) :
    QDialog(parent),
    ui(new Ui::LoginDialog) {
    ui->setupUi(this);

    //> make the window boderless
    /// @quotation This function calls setParent() when changing
    /// the flags for a window, causing the widget to be hidden.
    /// You must call show() to make the widget visible again.. -Qt5 Document
    /// @note but if this function is in constructor, the window won't be hidden
    /// when initially calling show()
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //> if this is a fallback window, don't bind listen sockets again
    if (isFallback) {
        init2ndPhase();
        return;
    }

    //> initialize the client core, and deal with initialization errors
    connect(&ClientCore::getInstance(), SIGNAL(connectorListenStarted()),
            this, SLOT(connectorListenStarted()));
    connect(&ClientCore::getInstance(), SIGNAL(fileListenStarted()),
            this, SLOT(transceiverListenStarted()));
    connect(&ClientCore::getInstance(), SIGNAL(connectorFailedToListen()),
            this, SLOT(connectorListenFailed()));
    connect(&ClientCore::getInstance(), SIGNAL(fileFailedToListen()),
            this, SLOT(transceiverListenFailed()));

    //> init and wait for initialized
    isTransceiverListenStarted = false;
    isConnectorListenStarted = false;
    ClientCore::getInstance().init();
}

void LoginDialog::init2ndPhase() {

    //> connect send data signals
    connect(this, SIGNAL(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress,
                            quint16, quint64, BaseConnector::DataType, QByteArray)),
            ClientCore::getInstance().getConnector(), SLOT(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress,
                            quint16, quint64, BaseConnector::DataType, QByteArray)),
            Qt::QueuedConnection);

    //> TODO: connect unexpted error
    /// core ////////////
    ////////////// ui ///

    //> set window icon
    setWindowIcon(QIcon(resSysTrayIcon));

    //> set and start the login animation
    QLabel *label = ui->loginAnimation;
    QMovie *loginAnimation = new QMovie(resLoginAnimation);
    label->setMovie(loginAnimation);
    loginAnimation->start();

    //> add the title buttons and bind the events
    TitleButton *titleButton = new TitleButton(this);
    titleButton->move(QPoint(this->size().width() - titleButton->size().width(), 0));
    titleButton->show();
    //>> connect close event
    connect(titleButton->getCloseButton(), SIGNAL(clicked()), this, SLOT(closeWindow()), Qt::UniqueConnection);
    //>> connect minimize event
    connect(titleButton->getMinimizeButton(), SIGNAL(clicked()), this, SLOT(minimizeWindow()), Qt::UniqueConnection);
    //>> connect settings button
    connect(titleButton->getSettingButon(), SIGNAL(clicked()), this, SLOT(showSettingsDialog()), Qt::UniqueConnection);

    //> connect other buttons
    //>> connet register button event
    connect(ui->registerButton, SIGNAL(clicked()), this, SLOT(showRegisterDialog()), Qt::UniqueConnection);
    //>> connet passwd findback button event
    connect(ui->passwordButton, SIGNAL(clicked()), this, SLOT(showPasswdDialog()), Qt::UniqueConnection);

    //> connet login button signal
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(loginButtonPressed()), Qt::UniqueConnection);

    //> add the system tary icon and connect signals
    trayIcon = new SystemTray(this);
    trayIcon->setTrayIcon(resSysTrayIcon);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)), Qt::UniqueConnection);

    //> init necessary paramters
    windowRestored = false;
    loginOperation = nullptr;

    // TODO TEST
    changeAvatar(":/img/icon.jpg");
    //TEST
}

LoginDialog::~LoginDialog() {
    delete ui;
    delete trayIcon;
}

// drag-to-move functions
void LoginDialog::mousePressEvent(QMouseEvent *event) {
    event->accept();
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event) {
    event->accept();

    // if window just restored, don't move
    if (windowRestored) {
        windowRestored = false;
        return;
    }
    if (event->buttons() & Qt::LeftButton)
        move(event->globalPos() - dragStartPosition);
}

void LoginDialog::connectorListenStarted() {
    isConnectorListenStarted = true;
    if (isTransceiverListenStarted == true)
        init2ndPhase();
}

void LoginDialog::connectorListenFailed() {
    //> show error
    QMessageBox *errorBox = new QMessageBox(this);
    errorBox->setText("An internal error occurred!");
    errorBox->setDetailedText("Detail: CONNECTOR_LISTEN_FAILED, please contact the developer");
    connect(errorBox, SIGNAL(finished(int)), this, SLOT(close()));
    errorBox->show();
}

void LoginDialog::transceiverListenStarted() {
    isTransceiverListenStarted = true;
    if (isConnectorListenStarted == true)
        init2ndPhase();
}

void LoginDialog::transceiverListenFailed() {
    //> show error
    QMessageBox *errorBox = new QMessageBox(this);
    errorBox->setText("An internal error occurred!");
    errorBox->setDetailedText("Detail: TRANSCEIVER_LISTEN_FAILED, please contact the developer");
    connect(errorBox, SIGNAL(finished(int)), this, SLOT(close()));
    errorBox->show();
}

// public methods
/////////////////////////

bool LoginDialog::changeAvatar(QString path) {
    QLabel *avatar = ui->Avatar;
    QImage image(path);

    if (image.isNull()) {
        qFatal("Failed to load image");
        return false;
    }

    //> the TRUE anti-aliasing way to round a image
    //>> use the original image size
    QImage dest(image.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);

    //>> draw a hollow round
    painter.fillRect(dest.rect(), QBrush(Qt::black, Qt::SolidPattern));
    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));
    painter.drawEllipse(dest.rect());

    //>> apply the image to the mask
    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    painter.drawImage(0, 0, image);

    //>> scale and apply the image at last (only let system do the resizing so it will be TRUELY antialiasing)
    avatar->setPixmap(QPixmap::fromImage(dest));
    return true;
}

void LoginDialog::minimizeWindow() {
    // can only shown up again by clicking the tray icon
    hide();
    //this->setWindowState(Qt::WindowMinimized);
}

void LoginDialog::trayActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::DoubleClick:
            this->show();
            windowRestored = true;
            break;
        case QSystemTrayIcon::Trigger:
            // TODO: show the menu
            break;
        default:
            break;
    }
}

void LoginDialog::closeWindow() {
    close();
}

void LoginDialog::showRegisterDialog() {
    RegisterDialog *regDiag = new RegisterDialog(this);
    regDiag->setWindowModality(Qt::ApplicationModal);
    regDiag->show();
}

void LoginDialog::showSettingsDialog() {
    SettingsDialog *settings = new SettingsDialog(this);
    settings->show();
}

void LoginDialog::loginButtonPressed() {
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    loginOperation = operation;

    connect(operation.data(), SIGNAL(dataSent()),
            this, SLOT(dataSent()));
    connect(operation.data(), SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(pendingDataReceived(QByteArray)));
    connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(sendReceiveError(OperationDescriptor::Error)));

    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyEmail], ui->LineEdit_email->text());
    sendJson.insert(PC::keyMap[PC::keyPasswd], ui->lineEdit_password->text());
    sendJson.insert(PC::keyMap[PC::keyPort], ClientCore::getInstance().messagePort());

    emit trySendData(operation, ClientCore::getInstance().serverAddress(), ClientCore::getInstance().serverPort(),
            operation->getId(), BaseConnector::typeLoginRequest, QJsonDocument(sendJson).toBinaryData());

    //> store myself statuSLOTs
    ClientCore::getInstance().setMyEmail(ui->LineEdit_email->text());
}

void LoginDialog::dataSent() {
    // do noting
}

void LoginDialog::pendingDataReceived(QByteArray rawData) {
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;
    BaseConnector::resolveData(rawData, type, reqNum, data);

    QJsonObject replyInfo = QJsonDocument::fromBinaryData(data).object();
    QString reply = replyInfo[PC::keyMap[PC::keyReply]].toString();

    qDebug().noquote() << "message received:" << QJsonDocument::fromBinaryData(data).toJson();

    // TODO: switch to global variable
    if (reply == PC::replyValueMap[PC::replyOK]) {
        //for ();, database::getInstance::...

        // TODO: add a loading line

        // BUG: there must be bugs here
        //> add friends to the database
        for (QString friendEmail : replyInfo[PC::keyMap[PC::keyFriend]].toObject().keys()) {
            QSharedPointer<DataBase::UserInfo> userInfo(new DataBase::UserInfo);
            QJsonObject friendInfo = replyInfo[PC::keyMap[PC::keyFriend]].toObject()[friendEmail].toObject();

            //> initialize the user
            userInfo->email = friendEmail;
            userInfo->username = friendInfo[PC::keyMap[PC::keyUserName]].toString();
            userInfo->avatar = friendInfo[PC::keyMap[PC::keyAvatar]].toString();
            userInfo->isOnline = friendInfo[PC::keyMap[PC::keyIsOnline]].toBool();
            if (userInfo->isOnline) {
                userInfo->address = QHostAddress(friendInfo[PC::keyMap[PC::keyAddress]].toString());
                userInfo->port = static_cast<quint16>(friendInfo[PC::keyMap[PC::keyPort]].toInt());
            }
            DataBase::getInstance().addUser(userInfo);
        }

        //> reterieve unread email to the database
        QJsonObject messagesObject = replyInfo[PC::keyMap[PC::keyMessage]].toObject();
        for (QString friendEmail : messagesObject.keys()) {
            QJsonArray messages = messagesObject[friendEmail].toArray();
            for (QJsonValue message : messages) {
                DataBase::getInstance().addMessageToLog(friendEmail, message.toString(), true);
            }
        }
        QJsonObject filesObject = replyInfo[PC::keyMap[PC::keyFile]].toObject();
        for (QString friendEmail : filesObject.keys()) {
            QJsonArray files = filesObject[friendEmail].toArray();
            for (QJsonValue file : files) {
                DataBase::getInstance().addFileToLog(friendEmail, file.toString(), true);
            }
        }

        MainWindow *mainWindow(new MainWindow(nullptr));
        mainWindow->show();

        close();
    } else if (reply == PC::replyValueMap[PC::replyIncorrectPassword]) {
        //> show wrong password hint
        ui->statusLabel->setText("Wrong password.");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    } else if (reply == PC::replyValueMap[PC::replyUserNotRegistered]) {
        ui->statusLabel->setText("User Not Registered");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    } else if (reply == PC::replyValueMap[PC::replyUserAlreadyOnline]) {
        ui->statusLabel->setText("Already online");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    }
}

void LoginDialog::sendReceiveError(OperationDescriptor::Error) {
    ui->statusLabel->setText("Connect Error");
    Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
}

void LoginDialog::showPasswdDialog() {
    if (!ClientCore::getInstance().isServerConnected()) {
        ui->statusLabel->setText("server not connected");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
        return;
    }
    //> init and show the password changing dialog
    PasswordDialog *passwordDialog = new PasswordDialog(this);
    passwordDialog->show();
}
