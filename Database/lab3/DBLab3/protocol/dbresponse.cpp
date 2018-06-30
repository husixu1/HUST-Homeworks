#include "dbresponse.h"

QJsonObject DBResponse::getTable() const {
    return table;
}

void DBResponse::setTable(const QJsonObject &value) {
    table = value;
}

DBResponse::CommonErrorCode DBResponse::getCommonError() const {
    return commonError;
}

void DBResponse::setCommonError(const CommonErrorCode &value) {
    commonError = value;
}

DBResponse::ServiceErrorCode DBResponse::getServiceError() const {
    return serviceError;
}

void DBResponse::setServiceError(const ServiceErrorCode &value) {
    serviceError = value;
}

void DBResponse::registerMetaTypes() {
    qRegisterMetaType<DBResponse>();
}

QJsonObject DBResponse::toJson() {
    QJsonObject result = table;
    // insert error code. sql column name cannot contain space so this is safe
    result.insert("common Err", static_cast<int>(commonError));
    result.insert("service Err", static_cast<int>(serviceError));
    return result;
}

DBResponse DBResponse::formJson(const QJsonObject &object) {
    DBResponse response;
    response.commonError = static_cast<CommonErrorCode>(object.value("common Err").toInt());
    response.serviceError = static_cast<ServiceErrorCode>(object.value("service Err").toInt());
    response.table = object;
    response.table.remove("common Err");
    response.table.remove("service Err");
    return response;
}

void DBResponse::addToResponseTable(DBResponse::ResponseDataKey key, QJsonValue value) {
    table.insert(QString("ext ") + QString::number(static_cast<int>(key)), value);
}

QJsonValue DBResponse::getFromResponseTable(DBResponse::ResponseDataKey key) {
    return table.value(QString("ext ") + QString::number(static_cast<int>(key)));
}

DBResponse::DBResponse() :
    commonError(CommonErrorCode::noError),
    serviceError(ServiceErrorCode::noError) {
}
