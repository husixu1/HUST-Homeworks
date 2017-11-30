#ifndef CLICABLELABEL_H
#define CLICABLELABEL_H

#include<QLabel>
#include<QMouseEvent>
#include<QtDesigner/QtDesigner>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    ClickableLabel(QString text, QWidget *parent): QLabel(text, parent) {}
    ClickableLabel(QWidget *widget): QLabel(widget) {}
    ~ClickableLabel() {}

signals:
    void clicked();

protected:
    /// @brief override function, emit clicked(); events
    void mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
    }
};

#endif // CLICABLELABEL_H
