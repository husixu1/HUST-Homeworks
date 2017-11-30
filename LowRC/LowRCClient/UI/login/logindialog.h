#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "clientcore.h"
#include "operationdescriptor.h"
#include "baseconnector.h"
#include "protocolconstant.h"

#include "titlebutton.h"
#include "clickablelabel.h"
#include "systemtray.h"
#include "registerdialog.h"
#include "passworddialog.h"
#include "settingsdialog.h"
#include "animations.h"
#include "res.h"

#include "mainwindow.h"
#include "database.h"

#include <QMovie>
#include <QMouseEvent>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QDialog>
#include <QPoint>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QEasingCurve>
#include <QPropertyAnimation>
#include <QMutex>
#include <QDebug>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0, bool isFallback = false);
    ~LoginDialog();

    /// @brief change the avatar of the login box
    /// @param path the path of the avatar
    /// @return if the change process succeeded
    bool changeAvatar(QString path);

protected:
    // dragging function
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::LoginDialog *ui;

    /// @brief only one login at a time
    QSharedPointer<OperationDescriptor> loginOperation;

    /// @brief a record, serve for dragging function
    QPoint dragStartPosition;
    /// @brief a paramter used to prevent sudden move of the window after restoring
    bool windowRestored;

    /// @brief system tary icon
    SystemTray *trayIcon;

    /// @brief the lock of the animation
    //QMutex statusLabelAnimationLock;

    /// @brief a listen started flag
    bool isConnectorListenStarted;
    /// @brief a listen started flag
    bool isTransceiverListenStarted;

    /// @brief the second phase of initialization
    /// basically, the existence of this function is to wait for
    /// the connector and transceiver to initialize, so that every widget
    /// cant use them correctly
    void init2ndPhase();

private slots:
    // client core slots
    /// @brief the connector started to listen
    void connectorListenStarted();
    /// @brief the connector failed to listen
    void connectorListenFailed();
    /// @brief the transceiver started to listen
    void transceiverListenStarted();
    /// @brief the transceiver failed to listen
    void transceiverListenFailed();

    // UI slots
    /// @brief minimize the window
    void minimizeWindow();
    /// @brief close the window and delete the tary icon
    void closeWindow();
    /// @brief when tary icon is clicked
    void trayActivated(QSystemTrayIcon::ActivationReason);
    /// @brief show the register dialog
    void showRegisterDialog();
    /// @brief show the settings dialog
    void showSettingsDialog();

    /// @brief attempt to login
    void loginButtonPressed();

    /// @brief login data sent
    void dataSent();
    /// @brief login received
    void pendingDataReceived(QByteArray data);
    /// @brief login send/receive error
    void sendReceiveError(OperationDescriptor::Error error);

    /// @brief show the password find-back dialog
    void showPasswdDialog();
signals:
    /// @brief send the login data
    void trySendData(QSharedPointer<OperationDescriptor>,
            QHostAddress, quint16, quint64, BaseConnector::DataType, QByteArray);
};

#endif // LOGINDIALOG_H
