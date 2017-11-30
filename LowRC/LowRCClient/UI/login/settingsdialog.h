/// @file settingsdialog.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "clientcore.h"
#include "operationdescriptor.h"

#include "titlebutton.h"
#include "clickablelabel.h"

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

private:
    Ui::SettingsDialog *ui;

    /// @brief a record, serve for dragging function
    QPoint dragStartPosition;
    /// @brief a paramter used to prevent sudden move of the window after restoring
    bool windowRestored;

protected:
    // dragging function
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:

    /// @brief when the connect to host button is pressed
    void connectButtonPressed();

    /// @brief the server is connected
    void serverConnected();

    /// @brief the server connection is failed
    void serverConnectFailed(OperationDescriptor::Error error);

    /// @brief close this dialog
    void closeDialog();
};

#endif // SETTINGSDIALOG_H
