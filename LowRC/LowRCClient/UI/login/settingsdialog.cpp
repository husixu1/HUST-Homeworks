/// @file settingsdialog.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    //> make the window boderless and do not display on taskbar
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //> add the title buttons and bind the events
    TitleButton *titleButton = new TitleButton(this);
    titleButton->onlyUse(TitleButton::CloseButton);
    //>> show the title buttons (make sure show is before move)
    titleButton->show();
    titleButton->move(QPoint(this->size().width() - titleButton->size().width(), 0));
    //>> connect close event
    connect(titleButton->getCloseButton(), SIGNAL(clicked()), this, SLOT(closeDialog()), Qt::UniqueConnection);

    //> connect server connection core event
    connect(&ClientCore::getInstance(), SIGNAL(connectedToServer()),
            this, SLOT(serverConnected()));
    connect(&ClientCore::getInstance(), SIGNAL(failedToConnectServer(OperationDescriptor::Error)),
            this, SLOT(serverConnectFailed(OperationDescriptor::Error)));

    //> connect UI events
    //>> connect "connectToHost" event
    connect(ui->connectButton, SIGNAL(clicked(bool)),
            this, SLOT(connectButtonPressed()), Qt::UniqueConnection);
}

// drag-to-move functions
void SettingsDialog::mousePressEvent(QMouseEvent *event) {
    event->accept();
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
}

void SettingsDialog::mouseMoveEvent(QMouseEvent *event) {
    event->accept();

    // if window just restored, don't move
    if (windowRestored) {
        windowRestored = false;
        return;
    }
    if (event->buttons() & Qt::LeftButton)
        move(event->globalPos() - dragStartPosition);
}

SettingsDialog::~SettingsDialog() {

}

void SettingsDialog::connectButtonPressed() {
    QHostAddress address(ui->addressLineEdit->text());
    quint16 port = ui->portSpinBox->text().toUInt();
    ClientCore::getInstance().connectToServer(address, port);
    ui->connectButton->setEnabled(false);
    ui->statusLabel->setText("Connecting to server...");
}

void SettingsDialog::serverConnected() {
    ui->statusLabel->setText("Server connected.");
    ui->connectButton->setEnabled(true);
    close();
}

void SettingsDialog::serverConnectFailed(OperationDescriptor::Error error) {
    ui->statusLabel->setText(
            "Error: failed to connect to server, Error: "
            + QString::number(static_cast<quint32>(error)));

    ui->connectButton->setEnabled(true);
}

void SettingsDialog::closeDialog() {
    close();
}

