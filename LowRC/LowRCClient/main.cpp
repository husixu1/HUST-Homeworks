#include "clientcore.h"
#include "logindialog.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    //> start application
    QApplication a(argc, argv);
    //> always start with login dialog
    LoginDialog w;
    //MainWindow w;

    w.show();

    return a.exec();
}
