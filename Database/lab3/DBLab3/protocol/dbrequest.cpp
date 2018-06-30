#include "dbrequest.h"
#include <exception>

void DBRequest::registerMetaTypes() {
    qRegisterMetaType<DBRequest>();
}

DBRequest::RequestService DBRequest::getRequestService() const {
    return requestService;
}

void DBRequest::setRequestService(const RequestService &value) {
    requestService = value;
}

DBRequest::DBRequest() :
    role(Role::invalid),
    userName(""),
    password(""),
    requestService(RequestService::invalid) {
}

QString DBRequest::getUserName() const {
    return userName;
}

void DBRequest::setUserName(const QString &value) {
    userName = value;
}

QString DBRequest::getPassword() const {
    return password;
}

void DBRequest::setPassword(const QString &value) {
    password = value;
}

DBRequest::Role DBRequest::getRole() const {
    return role;
}

void DBRequest::setRole(const Role &value) {
    role = value;
}

QJsonObject DBRequest::toJson() const {
    // add additional data
    QJsonObject result = requestData;

    // add mandatory data
    result.insert("_role", static_cast<int>(role));
    result.insert("_user", userName);
    result.insert("_pass", password);
    result.insert("_serv", static_cast<int>(requestService));

    return result;
}

DBRequest DBRequest::fromJson(const QJsonObject &object) {
    DBRequest result;

    // restore and remove mandatory data
    result.role = static_cast<Role>(object.value("_role").toInt());
    result.userName = object.value("_user").toString();
    result.password = object.value("_pass").toString();
    result.requestService = static_cast<RequestService>(object.value("_serv").toInt());

    result.requestData = object;
    result.requestData.remove("_role");
    result.requestData.remove("_user");
    result.requestData.remove("_pass");
    result.requestData.remove("_serv");

    return result;
}

void DBRequest::addToRequestData(DBRequest::RequestDataKey key, QJsonValue value) {
    // QJsonObject's key must be string
    requestData.insert(QString::number(static_cast<int>(key)), value);
}

QJsonValue DBRequest::getValFromRequestData(RequestDataKey dataKey) const {
    QString key = QString::number(static_cast<int>(dataKey));
    return requestData.value(key);
}
