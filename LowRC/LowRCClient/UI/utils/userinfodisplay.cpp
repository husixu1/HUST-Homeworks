/// @file userinfodisplay.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "userinfodisplay.h"
#include "ui_userinfodisplay.h"

QString UserInfoDisplay::getUserEmail() const {
    return userEmail;
}

void UserInfoDisplay::setUserEmail(const QString &value) {
    userEmail = value;
}

void UserInfoDisplay::mousePressEvent(QMouseEvent *event) {
    event->accept();
    qDebug() << "user " << userEmail << " pressed";
    emit userInfoPressed(userEmail);
}

void UserInfoDisplay::enterEvent(QEvent *event) {
    event->accept();
    this->setStyleSheet("background-color: rgb(136, 181, 226);");
    this->update();
}

void UserInfoDisplay::leaveEvent(QEvent *event) {
    event->accept();
    this->setStyleSheet("");
    this->update();
}

UserInfoDisplay::UserInfoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoDisplay) {
    ui->setupUi(this);
}

UserInfoDisplay::~UserInfoDisplay() {
    delete ui;
}

void UserInfoDisplay::setUserName(QString username) {
    ui->label_userName->setText(username);
}

void UserInfoDisplay::setMessage(QString message) {
    ui->label_message->setText(message);
}

void UserInfoDisplay::setIcon(QString icon) {
    //TODO
}
