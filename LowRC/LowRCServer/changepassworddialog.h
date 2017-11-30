/// @file changepassworddialog.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog {
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = 0);
    ~ChangePasswordDialog();

    /// @brief disable the old password line edit
    /// this is for the first-time password setting use
    void disableOldPassword();

private:
    Ui::ChangePasswordDialog *ui;

private slots:
    /// @brief deal with the accepted button pressed
    void acceptButtonClicked();

signals:
    /// @brief when ok button is pressed
    void okClicked(QString oldpw, QString newpw);
    /// @brief user canceled this dialog
    void canceled();
};

#endif // CHANGEPASSWORDDIALOG_H
