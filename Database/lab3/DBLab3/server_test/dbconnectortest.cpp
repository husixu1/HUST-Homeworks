#include "dbconnectortest.h"
#include "globalconfig.h"

DBConnectorTest::DBConnectorTest(QObject *parent) : QObject(parent) {

}

void DBConnectorTest::initTestCase() {

}

void DBConnectorTest::cleanupTestCase() {

}

void DBConnectorTest::connect() {
    DBConnector connector;
    QVERIFY(connector.startConnection("localhost", Config::databaseUserName, Config::databasePassword));
    QVERIFY(connector.stopConnection());
}
