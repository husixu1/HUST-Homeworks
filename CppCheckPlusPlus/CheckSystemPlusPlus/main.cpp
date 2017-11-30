/// @file main.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "mainwindow.h"
#include "res.h"

#include <QApplication>
#include <QTranslator>
#include <QFontDatabase>
#include <QtDebug>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // load font
    int id1 = QFontDatabase::addApplicationFont(":/res/FiraCode-Bold.ttf");
    int id2 = QFontDatabase::addApplicationFont(":/res/FiraCode-Light.ttf");
    int id3 = QFontDatabase::addApplicationFont(":/res/FiraCode-Medium.ttf");
    int id4 = QFontDatabase::addApplicationFont(":/res/FiraCode-Regular.ttf");
    Q_ASSERT(id1 != -1 && id2 != -1 && id3 != -1 && id4 != -1);

    MainWindow w;
    w.show();

    return a.exec();
}
