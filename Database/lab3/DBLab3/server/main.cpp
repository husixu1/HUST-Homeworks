#include "mainwindow.h"
#include "servicemanager.h"
#include "dbconnector.h"
#include "httpserver.h"
#include "dbrequest.h"
#include "dbresponse.h"
#include "qhttpserver.hpp"
#include "globalconfig.h"

#include <QApplication>
#include <QThread>
#include <QObject>

#include "SQLParser.h"

// declare metatypes
Q_DECLARE_METATYPE(qhttp::server::QHttpResponse *)

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // register metatypes
    DBRequest::registerMetaTypes();
    DBResponse::registerMetaTypes();
    qRegisterMetaType<qhttp::server::QHttpResponse *>();

    // set debug format
    qSetMessagePattern(Config::messagePattern);

    // initialize all services
    ServiceManager::getInstance().initialize();

    MainWindow w;
    // TODO: display logs here
    //w.show();
    return a.exec();
}
