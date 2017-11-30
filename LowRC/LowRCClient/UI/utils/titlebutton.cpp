#include "titlebutton.h"
#include "ui_titlebutton.h"


TitleButton::TitleButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleButton) {
    ui->setupUi(this);
}

TitleButton::~TitleButton() {
    delete ui;
}

void TitleButton::onlyUse(Button buttons) {
    ClickableLabel *temp;
    if (!(buttons & CloseButton)) {
        temp = ui->CloseButton;
        this->layout()->removeWidget(temp);
        delete temp;
    }
    if (!(buttons & minimizeButton)) {
        temp = ui->MinimizeButton;
        this->layout()->removeWidget(temp);
        delete temp;
    }
    if (!(buttons & settingButon)) {
        temp = ui->SettingsButton;
        this->layout()->removeWidget(temp);
        delete temp;
    }
    this->layout()->update();
}

ClickableLabel *TitleButton::getCloseButton() {
    return ui->CloseButton;
}

ClickableLabel *TitleButton::getMinimizeButton() {
    return ui->MinimizeButton;
}

ClickableLabel *TitleButton::getSettingButon() {
    return ui->SettingsButton;
}
