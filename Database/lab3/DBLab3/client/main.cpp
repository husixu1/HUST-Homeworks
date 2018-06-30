#include "dbrequest.h"
#include "dbresponse.h"
#include "globalconfig.h"

#include "logincontroller.h"
#include "studentcontroller.h"
#include "teachercontroller.h"
#include "managercontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QThread>

int main(int argc, char *argv[]) {
    // defualt
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // switch to metrial style
    QQuickStyle::setStyle("Material");

    // set debug format
    qSetMessagePattern(Config::messagePattern);

    // register meta types
    DBRequest::registerMetaTypes();
    DBResponse::registerMetaTypes();

    // register necessary types
    LoginController::registerQmlTypes();
    StudentController::registerQmlTypes();
    TeacherController::registerQmlTypes();
    ManagerController::registerQmlTypes();

    // start http client in another thread
    // TODO: close thread properly
    QThread clientThread;
    HttpClient::getInsatance().moveToThread(&clientThread);
    clientThread.start();

    // start the application
    QGuiApplication app(argc, argv);
    Config::qmlEngine = new QQmlApplicationEngine;
    Config::qmlEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (Config::qmlEngine->rootObjects().isEmpty())
        return -1;

    return app.exec();
}
