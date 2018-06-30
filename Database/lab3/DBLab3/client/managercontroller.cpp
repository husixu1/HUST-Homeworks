#include "managercontroller.h"
#include "globalconfig.h"
#include "protocolconstants.h"
#include "dbrequest.h"
#include "dbresponse.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

void ManagerController::refreshCourseModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "courseModel", QVariant::fromValue(coursesInfo));
}

void ManagerController::refreshTeacherModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "teacherModel", QVariant::fromValue(teachersInfo));
}

void ManagerController::refreshStudentmodel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "studentModel", QVariant::fromValue(studentsInfo));
}

void ManagerController::refreshTeacherAppModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "teacherAppModel", QVariant::fromValue(teacherApplicationList));
}

void ManagerController::refreshStudentAppModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "studentAppModel", QVariant::fromValue(studentApplicationList));
}

DBRequest ManagerController::constructBasicRequest() {
    DBRequest request;
    request.setUserName(username);
    request.setPassword(password);
    request.setRole(DBRequest::Role::manager);
    return request;
}

ManagerController::ManagerController(QObject *parent) : QObject(parent) {
    // connect process request handler
    connect(&HttpClient::getInsatance(), SIGNAL(serverResponsed(quint64)),
            this, SLOT(serverResponseHandler(quint64)),
            static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
}

void ManagerController::registerQmlTypes() {
    qmlRegisterType<ManagerController>(
            "DBLab3.ManagerController", 0, 1, "ManagerController");
}

void ManagerController::initialize() {
    getCoursesList();
    getStudentsList();
    getTeachersList();
    getTeacherApplicationList();
    getStudentApplicationList();

    refreshCourseModel();
    refreshStudentmodel();
    refreshTeacherModel();
    refreshTeacherAppModel();
    refreshStudentAppModel();
}

void ManagerController::getStudentsList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getStudents);
    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getStudentsList});
}

void ManagerController::getTeachersList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getTeachers);
    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getTeachersList});
}

void ManagerController::getCoursesList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getCourse);
    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getCoursesList});
}

void ManagerController::getTeacherApplicationList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getTeacherAppList);
    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getTeacherAppList});
}

void ManagerController::getStudentApplicationList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getStudentAppList);
    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getStudentAppList});
}

void ManagerController::executeStudentAppList(bool accept) {
    DBRequest request = constructBasicRequest();
    if (accept)
        request.setRequestService(DBRequest::RequestService::approveApplication);
    else
        request.setRequestService(DBRequest::RequestService::declineApplication);

    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::appRole,
            Config::ApplicationRole::student);
    QJsonArray ids;
    for (QString appId : studentAppPreparePool)
        ids.append(appId);
    request.addToRequestData(DBRequest::RequestDataKey::appIds, ids);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::executeStudentApplication});
}


void ManagerController::executeTeacherAppList(bool accept) {
    DBRequest request = constructBasicRequest();
    if (accept)
        request.setRequestService(DBRequest::RequestService::approveApplication);
    else
        request.setRequestService(DBRequest::RequestService::declineApplication);

    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::appRole,
            Config::ApplicationRole::teacher);
    QJsonArray ids;
    for (QString appId : teacherAppPreparePool)
        ids.append(appId);
    request.addToRequestData(DBRequest::RequestDataKey::appIds, ids);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::executeTeacherApplication});
}

void ManagerController::serverResponseHandler(quint64 token) {
    // if this request is sent by us
    if (requestPool.contains(token)) {
        // get response data and remove cached request token
        DBResponse response = HttpClient::getInsatance().getResponse(
                        requestPool.value(token).first);
        RequestType type = requestPool.value(token).second;
        requestPool.remove(token);

        // process common error
        switch (response.getCommonError()) {
            case DBResponse::CommonErrorCode::noError:
                break;
            case DBResponse::CommonErrorCode::internalError:
                emit displayError("Server internal error.");
                return;
            case DBResponse::CommonErrorCode::serverBusy:
                emit displayError("Server busy.");
                return;
            case DBResponse::CommonErrorCode::transactionFailed:
                emit displayError("Transaction failed");
                return;
            case DBResponse::CommonErrorCode::partialInternalError:
                emit displayError("Internal error on half way");
                break;
        }

        // process result
        switch (type) {
            case RequestType::getCoursesList: {
                QJsonObject table = response.getTable();
                coursesId.clear();
                coursesInfo.clear();
                for (int i = 0; i < table.value(Config::ColumnName::Course::id)
                        .toArray().size(); ++i) {
                    coursesId.append(table.value(Config::ColumnName::Course::id)
                            .toArray().at(i).toString());
                    coursesInfo.append(table.value(Config::ColumnName::Course::id)
                            .toArray().at(i).toString() + " " +
                            table.value(Config::ColumnName::Course::name)
                            .toArray().at(i).toString());
                }
                refreshCourseModel();
                break;
            }
            case RequestType::getStudentsList: {
                QJsonObject table = response.getTable();
                studentsId.clear();
                studentsInfo.clear();
                for (int i = 0; i < table.value(Config::ColumnName::Student::id)
                        .toArray().size(); ++i) {
                    studentsId.append(table.value(Config::ColumnName::Student::id)
                            .toArray().at(i).toString());
                    studentsInfo.append(table.value(Config::ColumnName::Student::id)
                            .toArray().at(i).toString() + " " +
                            table.value(Config::ColumnName::Student::name)
                            .toArray().at(i).toString());
                }
                refreshStudentmodel();
                break;
            }
            case RequestType::getTeachersList: {
                QJsonObject table = response.getTable();
                teachersId.clear();
                teachersInfo.clear();
                for (int i = 0; i < table.value(Config::ColumnName::Teacher::id)
                        .toArray().size(); ++i) {
                    teachersId.append(table.value(Config::ColumnName::Teacher::id)
                            .toArray().at(i).toString());
                    teachersInfo.append(table.value(Config::ColumnName::Teacher::id)
                            .toArray().at(i).toString() + " " +
                            table.value(Config::ColumnName::Teacher::name)
                            .toArray().at(i).toString());
                }
                refreshTeacherModel();
                break;
            }
            case RequestType::getTeacherAppList: {
                teacherApplicationIdList.clear();
                teacherApplicationList.clear();
                teacherAppPreparePool.clear();
                QJsonObject table = response.getTable();
                for (int i = 0; i < table
                        .value(Config::ColumnName::TeacherApplication::id)
                        .toArray().size(); ++i) {
                    teacherApplicationIdList.append(table.
                            value(Config::ColumnName::TeacherApplication::id)
                            .toArray().at(i).toString());
                    teacherApplicationList.append(table.
                            value(Config::ColumnName::TeacherApplication::statement)
                            .toArray().at(i).toString());
                }
                refreshTeacherAppModel();
                break;
            }
            case RequestType::getStudentAppList: {
                studentApplicationIdList.clear();
                studentApplicationList.clear();
                studentAppPreparePool.clear();
                QJsonObject table = response.getTable();
                for (int i = 0; i < table
                        .value(Config::ColumnName::StudentApplication::id)
                        .toArray().size(); ++i) {
                    studentApplicationIdList.append(table.
                            value(Config::ColumnName::StudentApplication::id)
                            .toArray().at(i).toString());
                    studentApplicationList.append(table.
                            value(Config::ColumnName::StudentApplication::statement)
                            .toArray().at(i).toString());
                }
                refreshStudentAppModel();
                break;
            }
            case RequestType::executeStudentApplication: {
                // refresh the list
                getStudentApplicationList();
                if (response.getServiceError()
                        != DBResponse::ServiceErrorCode::noError)
                    emit displayError("Some application failed to be executed");
                displayMessage("All application executed");
                break;
            }
            case RequestType::executeTeacherApplication: {
                getTeacherApplicationList();
                displayMessage("All application executed");
                break;
            }
            default:
                break;
        }
    }
}

void ManagerController::prepareTeacherApp(int index, bool checked) {
    qDebug() << "teacher app index " << index << (checked ? "checked" : "unchecked");
    if (checked)
        teacherAppPreparePool.insert(teacherApplicationIdList.at(index));
    else
        teacherAppPreparePool.remove(teacherApplicationIdList.at(index));
}

void ManagerController::prepareStudentApp(int index, bool checked) {
    qDebug() << "student app index " << index << (checked ? "checked" : "unchecked");
    if (checked)
        studentAppPreparePool.insert(studentApplicationIdList.at(index));
    else
        studentAppPreparePool.remove(studentApplicationIdList.at(index));
}
