#ifndef DBCONNECTORTEST_H
#define DBCONNECTORTEST_H

#include <QObject>
#include <QtTest>
#include "dbconnector.h"

class DBConnectorTest : public QObject {
    Q_OBJECT
public:
    explicit DBConnectorTest(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void connect();
};

#endif // DBCONNECTORTEST_H
