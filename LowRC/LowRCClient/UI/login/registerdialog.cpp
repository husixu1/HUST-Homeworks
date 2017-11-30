#include "registerdialog.h"
#include "ui_registerdialog.h"

namespace PC = ProtocolConstant;

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog) {
    ui->setupUi(this);

    //> make the window boderless and do not display on taskbar
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

//    //> add shadow to the widget
//    setAttribute(Qt::WA_TranslucentBackground);
//    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
//    effect->setBlurRadius(3);
//    ui->shadowWidget->setGraphicsEffect(effect);

    //> add the title buttons and bind the events
    TitleButton *titleButton = new TitleButton(this);
    titleButton->onlyUse(TitleButton::CloseButton);
    //>> show the title buttons (make sure show is before move)
    titleButton->show();
    titleButton->move(QPoint(this->size().width() - titleButton->size().width(), 0));
    //>> connect close event
    connect(titleButton->getCloseButton(), SIGNAL(clicked()), this, SLOT(closeDialog()), Qt::UniqueConnection);

    //>> connect register event
    connect(ui->registerButton, SIGNAL(clicked(bool)), this, SLOT(registerPressed()));

    //> connect connector signals
    connect(this, SIGNAL(sendMessage(
                            QSharedPointer<OperationDescriptor>, QHostAddress,
                            quint16, quint64, BaseConnector::DataType, QByteArray)),
            ClientCore::getInstance().getConnector(), SLOT(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress,
                            quint16, quint64, BaseConnector::DataType, QByteArray)),
            Qt::QueuedConnection);

    connect(ClientCore::getInstance().getConnector(), SIGNAL(unexpectedError(
                            QHostAddress, quint16, BaseConnector::UnexpectedError)),
            this, SLOT(unexpectedError(
                            QHostAddress, quint16, BaseConnector::UnexpectedError)),
            Qt::QueuedConnection);
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

// drag-to-move functions
void RegisterDialog::mousePressEvent(QMouseEvent *event) {
    event->accept();
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
}

void RegisterDialog::mouseMoveEvent(QMouseEvent *event) {
    event->accept();

    // if window just restored, don't move
    if (windowRestored) {
        windowRestored = false;
        return;
    }
    if (event->buttons() & Qt::LeftButton)
        move(event->globalPos() - dragStartPosition);
}


// slot functions
/////////////////////

void RegisterDialog::closeDialog() {
    this->close();
}

void RegisterDialog::registerPressed() {
    QString email = ui->LineEdit_email->text();
    QString username = ui->LineEdit_username->text();
    QString passwd   = ui->LineEdit_passwd->text();
    QString question = ui->LineEdit_safeQuestion->text();
    QString answer   = ui->LineEdit_answer->text();

    BaseConnector::DataType type = BaseConnector::typeRegisterRequest;
    if (ClientCore::getInstance().isServerConnected()) {
        QJsonObject sendJson;

        //> get the text
        sendJson.insert(PC::keyMap[PC::keyEmail], email);
        sendJson.insert(PC::keyMap[PC::keyUserName], username);
        sendJson.insert(PC::keyMap[PC::keyPasswd], passwd);
        // TODO: to image
        sendJson.insert(PC::keyMap[PC::keyAvatar], "");
        sendJson.insert(PC::keyMap[PC::keySafeQuestion], question);
        sendJson.insert(PC::keyMap[PC::keySafeAnswer], answer);

        //> try send the message
        QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
        operationPool.insert(operation.data(), operation);
        //>> connect data related signals
        connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
                this, SLOT(sendReceiveError(OperationDescriptor::Error)));
        connect(operation.data(), SIGNAL(pendingDataReceived(QByteArray)),
                this, SLOT(pendingDataReceived(QByteArray)));
        connect(operation.data(), SIGNAL(dataSent()),
                this, SLOT(dataSent()));

        //>> send the request to the server
        emit sendMessage(operation, ClientCore::getInstance().serverAddress(), ClientCore::getInstance().serverPort(),
                operation->getId(), BaseConnector::typeRegisterRequest, QJsonDocument(sendJson).toBinaryData());

        //> temporarily disable the button
        ui->registerButton->setEnabled(false);
    } else {
        QMessageBox *messageBox = new QMessageBox(this);
        messageBox->setWindowTitle("Error");
        messageBox->setText("cannot connect to host, please check your server config and connection.");
        messageBox->show();
    }

    //emit sendMessage(operation, );
    //> TODO: deal with error
    //> TODO: generate rsa key pair
    //> TODO: send the package
    //> TODO: receive the package
}

void RegisterDialog::dataSent() {
    ui->registerButton->setEnabled(true);
    ui->statusLabel->setText("register request sent");
}

void RegisterDialog::pendingDataReceived(QByteArray rawData) {
    //> resolve data
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;
    BaseConnector::resolveData(rawData, type, reqNum, data);

    //> process data
    QJsonObject response = QJsonDocument::fromBinaryData(data).object();
    QString reply = response[PC::keyMap[PC::keyReply]].toString();
    if (reply == PC::replyValueMap[PC::replyOK]) {
        ui->statusLabel->setText("Congratulations! register succeeded.");
    } else if (reply == PC::replyValueMap[PC::replyAlreadyRegistered]) {
        ui->statusLabel->setText("User already registered, try Login.");
        Animations::shakeLabel(ui->statusLabel, 0, 15, 500);
    }
    //> remove the operation from pool
    OperationDescriptor *operaion = qobject_cast<OperationDescriptor *>(sender());
    operationPool.remove(operaion);
    //> set button enabled again
    ui->registerButton->setEnabled(true);
}

void RegisterDialog::sendReceiveError(OperationDescriptor::Error error) {
    ui->registerButton->setEnabled(true);
    //> set error messages
    if (error == OperationDescriptor::errNotConnected)
        ui->statusLabel->setText("Server not connected");
    else
        ui->statusLabel->setText("Error occurred, code: " + QString::number(static_cast<quint32>(error)));
    Animations::shakeLabel(ui->statusLabel, 0, 15, 500);

    //> remove the operation from pool
    OperationDescriptor *operaion = qobject_cast<OperationDescriptor *>(sender());
    operationPool.remove(operaion);
    //> set button enabled again
    ui->registerButton->setEnabled(true);
}

void RegisterDialog::unexpectedError(QHostAddress addr, quint16 port, BaseConnector::UnexpectedError error) {
    if (addr == ClientCore::getInstance().serverAddress() &&
            port == ClientCore::getInstance().serverPort()) {
        //> show message box //TODO: change it to popup
        ui->statusLabel->setText("Oops... Server disconnected.");
        Animations::shakeLabel(ui->statusLabel, 0, 15, 500);

        //> clear the pending send map
        operationPool.clear();
    }
    ui->registerButton->setEnabled(true);
}

