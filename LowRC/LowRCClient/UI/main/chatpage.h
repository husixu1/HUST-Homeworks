/// @file chatpage.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef CHATPAGE_H
#define CHATPAGE_H

#include "chatbubble.h"
#include <QWidget>
#include <QLabel>
#include <QFormLayout>

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget {
    Q_OBJECT

private:
    Ui::ChatPage *ui;

    /// @brief when will the newly added message be displayed (in pixel)
    quint64 curPosition;

    QLabel peerIcon;
    QLabel myIcon;

public:
    explicit ChatPage(QWidget *parent = 0);
    ~ChatPage();

    void setMyIcon();
    void setPeerIcon();

    /// @brief display a message for the chating peer on the screen
    void addMessageForPeer(QString message);
    /// @brief display a message for self side on the screen
    void addMessageForMe(QString message);
};

#endif // CHATPAGE_H
