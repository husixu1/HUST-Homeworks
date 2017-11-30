/// @file changepassworddialog.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog) {
    ui->setupUi(this);

    connect(ui->acceptButtons, SIGNAL(accepted()), this, SLOT(acceptButtonClicked()));
    connect(ui->acceptButtons, &QDialogButtonBox::rejected, [ = ]() {
        emit canceled();
        close();
    });
}

ChangePasswordDialog::~ChangePasswordDialog() {
    delete ui;
}

void ChangePasswordDialog::disableOldPassword() {
    ui->oldPasswordEdit->setEnabled(false);
}

void ChangePasswordDialog::acceptButtonClicked() {
    if (ui->newPasswordEdit->text() == ui->confirmNewPasswordEdit->text()) {
        if (!ui->newPasswordEdit->text().isEmpty()) {
            emit okClicked(ui->oldPasswordEdit->text(), ui->newPasswordEdit->text());
            close();
        } else {
            QMessageBox *message = new QMessageBox(this);
            message->setWindowTitle("Error");
            message->setText("new password cannot be empty");
            message->show();
        }
    } else {
        QMessageBox *message = new QMessageBox(this);
        message->setWindowTitle("Error");
        message->setText("password inconsistency");
        message->show();
    }
}
