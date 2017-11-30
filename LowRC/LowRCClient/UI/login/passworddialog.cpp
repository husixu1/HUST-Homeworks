/// @file passworddialog.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "passworddialog.h"
#include "ui_passworddialog.h"

namespace PC = ProtocolConstant;

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog) {
    ui->setupUi(this);

    //> disable all interactive widgets, and set the initial phase
    ui->emailEdit->setEnabled(false);
    ui->questionEdit->setEnabled(false);
    ui->passwordEdit->setEnabled(false);
    ui->passwordConfirmEdit->setEnabled(false);
    setPhase(phaseEmail);

    //> connect signals
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(forwardSubmitbuttonClicked()));

    //> connect send signals
    connect(this, SIGNAL(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress, quint16,
                            quint64, BaseConnector::DataType, QByteArray)),
            ClientCore::getInstance().getConnector(), SLOT(trySendData(
                            QSharedPointer<OperationDescriptor>, QHostAddress, quint16,
                            quint64, BaseConnector::DataType, QByteArray)),
            Qt::QueuedConnection);
    //> close this window if server error occurred
    connect(ClientCore::getInstance().getConnector(), &BaseConnector::unexpectedError,
    [ = ](QHostAddress address, quint16 port, BaseConnector::UnexpectedError) {
        if (address == ClientCore::getInstance().serverAddress() &&
                port == ClientCore::getInstance().serverPort())
            close();
    });
}

PasswordDialog::~PasswordDialog() {
    delete ui;
}

void PasswordDialog::setPhase(PasswordDialog::Phase phase) {
    currentPhase = phase;
    ui->emailEdit->setEnabled(false);
    ui->questionEdit->setEnabled(false);
    ui->passwordConfirmEdit->setEnabled(false);
    ui->passwordConfirmEdit->setEnabled(false);

    //> toggle enabled widgets
    switch (phase) {
        case phaseEmail:
            ui->emailEdit->setEnabled(true);
            break;
        case phaseQuestion:
            ui->questionEdit->setEnabled(true);
            break;
        case phasePassword:
            ui->passwordEdit->setEnabled(true);
            ui->passwordConfirmEdit->setEnabled(true);
            break;
    }
}

void PasswordDialog::setQuestion(QString question) {
    ui->questioLabel->setText(QString("Question: ") + question);
}

void PasswordDialog::setStatusBarError(QString error) {
    ui->statusLabel->setText(error);
    Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
}

void PasswordDialog::forwardSubmitbuttonClicked() {
    switch (currentPhase) {
        case phaseEmail:
            passwdChangePhase1(ui->emailEdit->text());
            break;
        case phaseQuestion:
            passwdChangePhase2(ui->questionEdit->text());
            break;
        case phasePassword: {
            //> forbid inconsistent password
            if (ui->passwordEdit->text() != ui->passwordConfirmEdit->text()) {
                ui->statusLabel->setText("password inconsistent");
                Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
                return;
            }

            //> forbid empty password
            if (ui->passwordEdit->text().isEmpty()) {
                ui->statusLabel->setText("password cannot be empty");
                Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
                return;
            }
            passwdChangePhase3(ui->passwordEdit->text());
            break;
        }
    }
}

void PasswordDialog::passwdChangePhase1(QString email) {
    //> forbid double-send
    ui->submitButton->setEnabled(false);
    userEmail = email;

    //> init operatoin
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    this->passwdChangeOperation = operation;

    //> connect signals
    connect(operation.data(), SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(passwdPhase1Replyed(QByteArray)));
    connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(passwdPhase1Error(OperationDescriptor::Error)));

    //> init send object
    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyEmail], email);

    //> send password changing stage/phase1 data
    emit trySendData(operation,
            ClientCore::getInstance().serverAddress(), ClientCore::getInstance().serverPort(),
            operation->getId(), BaseConnector::typePassword1Request,
            QJsonDocument(sendJson).toBinaryData());
}

void PasswordDialog::passwdPhase1Replyed(QByteArray rawData) {
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;

    //> resolve data
    BaseConnector::resolveData(rawData, type, reqNum, data);
    QJsonObject replyJson = QJsonDocument::fromBinaryData(data).object();

    if (replyJson[PC::keyMap[PC::keyReply]] == PC::replyValueMap[PC::replyOK]) {
        //> enter phase 2, set the question
        setPhase(PasswordDialog::phaseQuestion);
        ui->questioLabel->setText(replyJson[PC::keyMap[PC::keySafeQuestion]].toString());

        //> re-connect signals
        passwdChangeOperation->disconnect();
    } else if (replyJson[PC::keyMap[PC::keyReply]].toString() == PC::replyValueMap[PC::replyUserNotRegistered]) {
        ui->statusLabel->setText("user not registered");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    } else {
        ui->statusLabel->setText("unidentified Error: " + replyJson[PC::keyMap[PC::keyReply]].toString());
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    }
    //> re-enable no matter what phase it'll enter
    ui->submitButton->setEnabled(true);
}

void PasswordDialog::passwdPhase1Error(OperationDescriptor::Error error) {
    ui->statusLabel->setText(QString("failed to send data: ") + QString::number(error));
    Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
}

void PasswordDialog::passwdChangePhase2(QString answer) {
    //> forbid double-send
    ui->submitButton->setEnabled(false);

    //> init operatoin
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    this->passwdChangeOperation = operation;

    //> connect signals
    connect(operation.data(), SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(passwdPhase2Replyed(QByteArray)));
    connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(passwdPhase2Error(OperationDescriptor::Error)));

    //> init send object
    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyEmail], userEmail);
    sendJson.insert(PC::keyMap[PC::keySafeAnswer], answer);

    //> send password changing stage/phase1 data
    emit trySendData(operation,
            ClientCore::getInstance().serverAddress(), ClientCore::getInstance().serverPort(),
            operation->getId(), BaseConnector::typePassword2Request,
            QJsonDocument(sendJson).toBinaryData());
}

void PasswordDialog::passwdPhase2Replyed(QByteArray rawData) {
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;

    //> resolve data
    BaseConnector::resolveData(rawData, type, reqNum, data);
    QJsonObject replyJson = QJsonDocument::fromBinaryData(data).object();

    if (replyJson[PC::keyMap[PC::keyReply]].toString() == PC::replyValueMap[PC::replyOK]) {
        validateKey = replyJson[PC::keyMap[PC::keyValidate]].toInt();
        //> enter phase 3
        setPhase(phasePassword);

        //> re-connect signals
        passwdChangeOperation->disconnect();
    } else if (replyJson[PC::keyMap[PC::keyReply]].toString() == PC::replyValueMap[PC::replyWrongAnswer]) {
        ui->statusLabel->setText("wrong answer");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    } else {
        ui->statusLabel->setText("unidentified Error: " + replyJson[PC::keyMap[PC::keyReply]].toString());
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    }
    //> re-enable no matter what phase it'll enter
    ui->submitButton->setEnabled(true);
}

void PasswordDialog::passwdPhase2Error(OperationDescriptor::Error error) {
    ui->statusLabel->setText(QString("failed to send data: ") + QString::number(error));
    Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
}

void PasswordDialog::passwdChangePhase3(QString passwd) {
    //> forbid double-send
    ui->submitButton->setEnabled(false);

    //> init operatoin
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    this->passwdChangeOperation = operation;

    //> connect signals
    connect(operation.data(), SIGNAL(pendingDataReceived(QByteArray)),
            this, SLOT(passwdPhase3Replyed(QByteArray)));
    connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(passwdPhase3Error(OperationDescriptor::Error)));

    //> init send object
    QJsonObject sendJson;
    sendJson.insert(PC::keyMap[PC::keyEmail], userEmail);
    sendJson.insert(PC::keyMap[PC::keyPasswd], passwd);
    sendJson.insert(PC::keyMap[PC::keyValidate], static_cast<int>(validateKey));

    //> send password changing stage/phase1 data
    emit trySendData(operation,
            ClientCore::getInstance().serverAddress(), ClientCore::getInstance().serverPort(),
            operation->getId(), BaseConnector::typePassword3Request,
            QJsonDocument(sendJson).toBinaryData());
}

void PasswordDialog::passwdPhase3Replyed(QByteArray rawData) {
    BaseConnector::DataType type;
    quint64 reqNum;
    QByteArray data;

    //> resolve data
    BaseConnector::resolveData(rawData, type, reqNum, data);
    QJsonObject replyJson = QJsonDocument::fromBinaryData(data).object();

    if (replyJson[PC::keyMap[PC::keyReply]].toString() == PC::replyValueMap[PC::replyOK]) {
        close();
    } else if (replyJson[PC::keyMap[PC::keyReply]].toString() == PC::replyValueMap[PC::replyInvalidRequest]) {
        ui->statusLabel->setText("timed out, act quicker next time!");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    } else {
        ui->statusLabel->setText("update failed due to unknown power");
        Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
    }
}

void PasswordDialog::passwdPhase3Error(OperationDescriptor::Error error) {
    ui->statusLabel->setText(QString("failed to send data: ") + QString::number(error));
    Animations::shakeLabel(ui->statusLabel, 0, 10, 500);
}

