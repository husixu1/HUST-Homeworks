#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "clientcore.h"
#include "protocolconstant.h"

#include "titlebutton.h"
#include "clickablelabel.h"
#include "animations.h"

#include <QDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPoint>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>


namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();

protected:
    // dragging function
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::RegisterDialog *ui;

    /// @brief a record, serve for dragging function
    QPoint dragStartPosition;
    /// @brief a paramter used to prevent sudden move of the window after restoring
    bool windowRestored;

    /// @brief the OperationDescriptor's memory pool
    QMap<OperationDescriptor *, QSharedPointer<OperationDescriptor>> operationPool;

private slots:
    /// @brief the sent data is sent
    void dataSent();
    /// @brief the pending respond is received
    void pendingDataReceived(QByteArray data);
    /// @brief process error in sending and receving process
    void sendReceiveError(OperationDescriptor::Error error);

public slots:
    void closeDialog();
    void registerPressed();

    void unexpectedError(QHostAddress, quint16, BaseConnector::UnexpectedError);

signals:

    void sendMessage(QSharedPointer<OperationDescriptor> operation,
            QHostAddress addr, quint16 port, quint64 reqNum,
            BaseConnector::DataType type, QByteArray data);

};

#endif // REGISTERDIALOG_H
