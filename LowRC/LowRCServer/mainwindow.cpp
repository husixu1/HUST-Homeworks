/// @file mainwindow.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //> create connector and data processor, and connect them together
    ServerCore::getInstance().init();

    //> update related sinals
    updateTimer = new QTimer(this);
    connect(&DataBase::getInstance(), SIGNAL(dataBaseChanged()),
            this, SLOT(updateUserList()),
            Qt::QueuedConnection);
    connect(updateTimer, SIGNAL(timeout()),
            this, SLOT(updateTimedOut()));

    //> validate and load database button
    connect(ui->validatebutton, SIGNAL(clicked()),
            this, SLOT(validateButtonClicked()));
    connect(ui->changepasswdbutton, SIGNAL(clicked()),
            this, SLOT(changePasswdButtonClicked()));

    //> try load config
    isConfigLoaded = GlobalConfig::getInstance().loadConfig();

    //> init necessary variable
    needUpdate = false;
    //>> 0.1 sec per update initially
    updateTime = 100;
    updateTimer->start(updateTime);

    connect(ui->userList, SIGNAL(clicked(QModelIndex)), this, SLOT(userClicked(QModelIndex)));
}

MainWindow::~MainWindow() {
    delete ui;
    updateTimer->stop();
    delete updateTimer;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    //> store the database before close the window
    DataBase::getInstance().serialize();
    GlobalConfig::getInstance().saveConfig();
    event->accept();
}

void MainWindow::updateUserList() {
    needUpdate = true;
}

void MainWindow::updateTimedOut() {
    // only update in certain interval
    if (needUpdate) {
        //> renew the tree view
        QTreeWidget *userList = ui->userList;
        auto userDataList = DataBase::getInstance().getUserList();
        //> clear the list and re-add
        userList->clear();
        for (auto userData : userDataList) {
            QString addressPort = userData->isOnline ?
                    userData->address.toString()
                    + QString("@") + QString::number(userData->connectedPort)
                    + QString("@") + QString::number(userData->listenPort) :
                    QString("offline");
            QTreeWidgetItem *item = new QTreeWidgetItem({userData->email, userData->username, addressPort});
            userList->insertTopLevelItem(0, item);
        }
        if (!currentUserEmail.isEmpty()) {
            updateDetailPanelInfo(currentUserEmail);
        }

        //> reset the timer parameter
        needUpdate = false;
        updateTime = userDataList.size() * updateFactor + 100;
    }
    updateTimer->start(updateTime);
}

void MainWindow::userClicked(QModelIndex index) {
    if (!index.isValid())
        return;

    QString userEmail = index.sibling(index.row(), 0).data().toString();
    currentUserEmail = userEmail;

    updateDetailPanelInfo(userEmail);
}

void MainWindow::validateButtonClicked() {
    // if config file is not found, return an error message
    if (!GlobalConfig::getInstance().contains(GlobalConfig::keyEncryptKeyHash)) {
        QMessageBox *messageBox = new QMessageBox(this);
        messageBox->setWindowTitle("Error");
        messageBox->setText("passwd not set. cannot validate passwd.");
        messageBox->show();
    } else {
        PasswordDialog *passwdDialog = new PasswordDialog(this);
        connect(passwdDialog, SIGNAL(okClicked(QString)),
                this, SLOT(validatePasswd(QString)));
        passwdDialog->show();
    }
}

void MainWindow::validatePasswd(QString rawPasswd) {
    // get the stored passwd
    //> hashed text --base64-decode-> hashed
    QByteArray standardHashedPasswd = QByteArray::fromBase64(
                    GlobalConfig::getInstance()
                    .getValue(GlobalConfig::keyEncryptKeyHash)
                    .toString().toUtf8());
    //> origin --md5-> hashed
    QByteArray hashedPasswd = QCryptographicHash::hash(rawPasswd.toUtf8(), QCryptographicHash::Md5);

    //> compare two hashed passwd
    if (standardHashedPasswd == hashedPasswd) {
        //> if passed validation
        DataBase::getInstance().setEncryptKey(reinterpret_cast<const uint8_t *>(hashedPasswd.data()));
        DataBase::getInstance().deSerialize();
    } else {
        QMessageBox *wrongPasswd = new QMessageBox(this);
        wrongPasswd->setWindowTitle("Warning");
        wrongPasswd->setText("wrong password!");
        wrongPasswd->show();
    }
}

void MainWindow::changePasswdButtonClicked() {
    ChangePasswordDialog *changePasswdDialog = new ChangePasswordDialog(this);
    connect(changePasswdDialog, SIGNAL(okClicked(QString, QString)),
            this, SLOT(changePasswd(QString, QString)));
    if (!GlobalConfig::getInstance().contains(GlobalConfig::keyEncryptKeyHash)) {
        changePasswdDialog->disableOldPassword();
    }
    changePasswdDialog->show();
}

void MainWindow::changePasswd(QString oldPasswd, QString newPasswd) {
    QMessageBox *resultMessage = new QMessageBox(this);
    //> original --md5-> hashed --base64-> hashed text
    QByteArray hashedPasswd = QCryptographicHash::hash(newPasswd.toUtf8(), QCryptographicHash::Md5);
    QString hashedPasswdText = QString::fromUtf8(hashedPasswd.toBase64());

    if (GlobalConfig::getInstance().contains(GlobalConfig::keyEncryptKeyHash)) {
        //> load old passwd
        QByteArray standardHashedOldPasswd = QByteArray::fromBase64(
                        GlobalConfig::getInstance()
                        .getValue(GlobalConfig::keyEncryptKeyHash)
                        .toString().toUtf8());
        QByteArray hashedOldPassword = QCryptographicHash::hash(oldPasswd.toUtf8(), QCryptographicHash::Md5);

        if (hashedOldPassword == standardHashedOldPasswd) {
            GlobalConfig::getInstance().setVaule(GlobalConfig::keyEncryptKeyHash, hashedPasswdText);
            resultMessage->setWindowTitle("info");
            resultMessage->setText("password changed");

            DataBase::getInstance().setEncryptKey(reinterpret_cast<const uint8_t *>(hashedPasswd.data()));
        } else {
            resultMessage->setWindowTitle("error");
            resultMessage->setText("wrong password");
        }
    } else {
        //> initialize passowrd
        GlobalConfig::getInstance().setVaule(GlobalConfig::keyEncryptKeyHash, hashedPasswdText);
        resultMessage->setWindowTitle("info");
        resultMessage->setText("password set");
        DataBase::getInstance().setEncryptKey(reinterpret_cast<const uint8_t *>(hashedPasswd.data()));
    }
    resultMessage->show();
}

void MainWindow::updateDetailPanelInfo(QString userEmail) {
    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(userEmail);

    ui->detailTree->clear();

    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"email", userInfo->email}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"userName", userInfo->username}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"Address", userInfo->address.toString()}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"listenPort", QString::number(userInfo->listenPort)}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"connectPort", QString::number(userInfo->connectedPort)}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"isOnline", userInfo->isOnline ? "true" : "false"}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"safeQuestion", userInfo->safeQuestion}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"safeAnswer", userInfo->safeQuestionAnswer}));
    ui->detailTree->addTopLevelItem(new QTreeWidgetItem({"password", userInfo->passwd}));
    QTreeWidgetItem *friends = new QTreeWidgetItem({"friends"});
    for (auto i : userInfo->friends)
        friends->addChild(new QTreeWidgetItem({i}));
    ui->detailTree->addTopLevelItem(friends);

    QTreeWidgetItem *messages = new QTreeWidgetItem({"cachedMessages"});
    for (auto i : userInfo->cachedMessage)
        messages->addChild(new QTreeWidgetItem({i.first, i.second}));
    ui->detailTree->addTopLevelItem(messages);

    QTreeWidgetItem *files = new QTreeWidgetItem({"cachedFiles"});
    for (auto i : userInfo->cachedFile)
        files->addChild(new QTreeWidgetItem({i.first, i.second}));
}
