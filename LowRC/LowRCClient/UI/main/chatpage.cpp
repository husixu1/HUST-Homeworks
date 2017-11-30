/// @file chatpage.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "chatpage.h"
#include "ui_chatpage.h"

ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage) {
    ui->setupUi(this);
}

ChatPage::~ChatPage() {
    delete ui;
}

void ChatPage::addMessageForPeer(QString message) {
    //> create a new bubble and fill message
    ChatBubble *bubble = new ChatBubble;
    bubble->useDirection(ChatBubble::left);
    bubble->setMessage(message);

    //> add bubble to layout
    QHBoxLayout *newLine = new QHBoxLayout;
    newLine->setAlignment(bubble, Qt::AlignRight);
    newLine->addWidget(bubble);
    ui->chatPageLayout->addLayout(newLine, ui->chatPageLayout->rowCount(), 0, Qt::AlignLeft);

    //> show the bubble
    bubble->show();
}

void ChatPage::addMessageForMe(QString message) {
    //> create a new bubble and fill message
    ChatBubble *bubble = new ChatBubble;
    bubble->useDirection(ChatBubble::right);
    bubble->setMessage(message);

    //> add bubble to layout
    QHBoxLayout *newLine = new QHBoxLayout;
    newLine->setAlignment(bubble, Qt::AlignLeft);
    newLine->addWidget(bubble);
    ui->chatPageLayout->addLayout(newLine, ui->chatPageLayout->rowCount(), 0, Qt::AlignRight);

    //> show the bubble
    bubble->show();
}
