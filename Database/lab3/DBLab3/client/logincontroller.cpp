#include "logincontroller.h"
#include "dbrequest.h"
#include "dbresponse.h"
#include "httpclient.h"
#include "globalconfig.h"
#include "protocolconstants.h"

#include <QDebug>
#include <QQmlEngine>
#include <QCryptographicHash>

LoginController::LoginController(QObject *parent) : QObject(parent) {

}

void LoginController::registerQmlTypes() {
    // register qml types TODO: consider move it to another class
    qmlRegisterType<LoginController>("DBLab3.LoginController", 0, 1, "LoginController");
}

void LoginController::onServerResponse(quint64 token) {
    // if token matches
    if (token == *loginToken) {
        DBResponse response = HttpClient::getInsatance().getResponse(loginToken);
        if (response.getCommonError() == DBResponse::CommonErrorCode::noError &&
                response.getServiceError() == DBResponse::ServiceErrorCode::noError) {
            qInfo("Login successful");
            // login to another window
            emit login(
                    lastSelectedRole,
                    lastUsername,
                    lastEncryptedPass,
                    response.getTable().value(Config::ColumnName::Student::id).toString(),
                    response.getTable().value(Config::ColumnName::Student::name).toString());
        } else {
            // Display information
            qInfo() << "Login failed, common Error: " << response.getCommonError()
                    << ", Service Error: " << response.getServiceError();
            switch (response.getCommonError()) {
                case DBResponse::CommonErrorCode::noError:
                    break;
                case DBResponse::CommonErrorCode::internalError:
                    emit displayError("Server internal error.");
                    return;
                case DBResponse::CommonErrorCode::serverBusy:
                    emit displayError("Server is busy, try later.");
                    break;
                default:
                    emit displayError("Unknown error");
                    break;
            }

            switch (response.getServiceError()) {
                case DBResponse::ServiceErrorCode::noError:
                    return;
                case DBResponse::ServiceErrorCode::noUser:
                    emit displayError("User not registered.");
                    return;
                case DBResponse::ServiceErrorCode::wrongPassword:
                    emit displayError("Wrong password.");
                    return;
                default:
                    emit displayError("Client error, contact software vendor.");
                    return;
            }
        }
    }
}

void LoginController::tryLogin(QString username, QString password, Role role) {
    qInfo() << "try to login with username: "
            << username << ", password: " << password << ", role: " << role;
    DBRequest request;

    // fill in the form
    request.setUserName(username);
    // convert password to md5 hash
    QCryptographicHash hasher(QCryptographicHash::Algorithm::Md5);
    hasher.addData(password.toUtf8());
    request.setPassword(hasher.result().toHex());
    request.setRequestService(DBRequest::RequestService::login);
    if (role == Role::Manager)
        request.setRole(DBRequest::Role::manager);
    else if (role == Role::Teacher)
        request.setRole(DBRequest::Role::teacher);
    else if (role == Role::Student)
        request.setRole(DBRequest::Role::student);
    else
        qCritical("role unrecognized, program should not run here");

    // store informations temporarily
    lastSelectedRole = role;
    lastUsername = username;
    lastEncryptedPass = hasher.result().toHex();

    // send the request, use unique connection
    connect(&HttpClient::getInsatance(), SIGNAL(serverResponsed(quint64)),
            this, SLOT(onServerResponse(quint64)),
            static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::QueuedConnection));
    loginToken = HttpClient::getInsatance().sendRequest(request);
}

