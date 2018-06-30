#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "servicemanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *) {
    // wait for event process and close
    connect(&ServiceManager::getInstance(), SIGNAL(serviceStopped()),
            this, SLOT(close()));
    ServiceManager::getInstance().stopAndClenup();
}
