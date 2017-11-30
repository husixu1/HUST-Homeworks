/// @file passworddialog.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog) {
    ui->setupUi(this);

    //> connect signals
    connect(ui->selection, SIGNAL(accepted()), this, SLOT(accepted()));
    connect(ui->selection, &QDialogButtonBox::rejected, [ = ]() {
        emit canceled();
        close();
    });
}

PasswordDialog::~PasswordDialog() {
    delete ui;
}

void PasswordDialog::accepted() {
    QString passwd = ui->passwordEdit->text();
    emit okClicked(passwd);
    close();
}
