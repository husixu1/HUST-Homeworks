#include<QApplication>
#include<QString>
#include<QTest>

#include "dbconnectortest.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTest::qExec(new DBConnectorTest(), argc, argv);
    return 0;
}

