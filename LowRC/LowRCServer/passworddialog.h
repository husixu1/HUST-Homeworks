/// @file passworddialog.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog {
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();

private:
    Ui::PasswordDialog *ui;

private slots:
    void accepted();

signals:
    /// @brief when the ok button is pressed, pass the password
    void okClicked(QString passwd);
    /// @brief when the dialog is canceled
    void canceled();
};

#endif // PASSWORDDIALOG_H
