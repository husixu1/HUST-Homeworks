/// @file chatbubble.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "chatbubble.h"
#include "ui_chatbubble.h"

ChatBubble::ChatBubble(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBubble) {
    ui->setupUi(this);
}

ChatBubble::~ChatBubble() {
    delete ui;
}

void ChatBubble::useDirection(ChatBubble::Direction direction) {
    switch (direction) {
        case left:
            ui->rightArrowLayout->removeWidget(ui->rightArrow);
            ui->rightArrowLayout->removeItem(ui->rightSpacer);
            delete ui->rightArrowLayout;
            break;
        case right:
            ui->leftArrowLayout->removeWidget(ui->leftArrow);
            ui->leftArrowLayout->removeItem(ui->leftSpacer);
            delete ui->leftArrowLayout;
            break;
    }
}

void ChatBubble::setMessage(QString text) {
    ui->message->setText(text);
}
