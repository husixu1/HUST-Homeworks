/// @file chatbubble.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QWidget>

namespace Ui {
class ChatBubble;
}

class ChatBubble : public QWidget {
    Q_OBJECT

public:
    enum Direction {
        left,
        right,
    };
private:
    Ui::ChatBubble *ui;

public:
    explicit ChatBubble(QWidget *parent = 0);
    ~ChatBubble();

    void useDirection(Direction direction);
    void setMessage(QString text);
};

#endif // CHATBUBBLE_H
