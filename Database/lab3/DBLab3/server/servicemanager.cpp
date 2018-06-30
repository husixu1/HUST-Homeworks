#include "servicemanager.h"

#include <QtDebug>

ServiceManager::ServiceManager(QObject *parent) : QObject(parent) {
    // thread running dbconnector
    dbThread = new QThread;
    dbConnector = new DBConnector("1");
    // make database connector in another thread
    dbConnector->moveToThread(dbThread);

    httpServer = new HttpServer;
}

ServiceManager &ServiceManager::getInstance() {
    static ServiceManager instance;
    return instance;
}

void ServiceManager::initialize() {
    qInfo("try to start all services");
    // TODO: change to multiple connector
    // start database connector
    dbThread->start();
    dbConnector->startConnectionLater("localhost", "dblab", "dblabdblab");

    // start http server
    httpServer->startServer(8080);
    httpServer->addDBConnector(dbConnector);
}

void ServiceManager::stopAndClenup() {
    qInfo("try to stop all services");
    dbConnector->stopConnectionLater();

    // exit thread after finished processing remaining events
    connect(dbConnector, &DBConnector::connectionClosed, [ = ]() {
        connect(dbThread, SIGNAL(finished()), this, SIGNAL(serviceStopped()));
        dbThread->exit();
    });
}

ServiceManager::~ServiceManager() {
    // release resources
    delete dbThread;
    delete dbConnector;
    delete httpServer;
}

HttpServer *ServiceManager::getHttpServer() const {
    return httpServer;
}
