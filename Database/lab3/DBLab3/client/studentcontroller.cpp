#include "studentcontroller.h"
#include "globalconfig.h"
#include "dbrequest.h"
#include "dbresponse.h"
#include "httpclient.h"
#include "protocolconstants.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>
#include <QJsonArray>

StudentController::StudentController(QObject *parent) : QObject(parent) {
    // connect process request handler
    connect(&HttpClient::getInsatance(), SIGNAL(serverResponsed(quint64)),
            this, SLOT(serverResponseHandler(quint64)),
            static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection)
    );
}

void StudentController::registerQmlTypes() {
    qmlRegisterType<StudentController>("DBLab3.StudentController", 0, 1, "StudentController");
}

void StudentController::refreshSelectedCourseModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "selectedCourseModel", QVariant::fromValue(selectedCourseList));
}

void StudentController::exportAllCourseModel() {
    filteredCourses.setSourceModel(&allCourses);
    Config::qmlEngine->rootContext()->setContextProperty(
            "allCourseModel", &filteredCourses);
}

void StudentController::refreshRelaventTaskModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "relaventTaskModel", QVariant::fromValue(relaventTaskList));
}
void StudentController::refreshRelaventApplicationModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "relaventAppModel", QVariant::fromValue(relaventApplicationList));
}

void StudentController::initialize() {
    // initialize page
    getSelectedCourse();
    getRelaventTaskList();
    getRelaventApplicationList();

    // initialize models
    refreshSelectedCourseModel();
    refreshRelaventTaskModel();
    refreshRelaventApplicationModel();
    exportAllCourseModel();
}

bool StudentController::getIsCourseSelected(QString courseId) {
    return allCourses.getIsSelected(courseId);
}

bool StudentController::getIsCoursePending(QString courseId) {
    return allCourses.getIsPending(courseId);
}

void StudentController::getSelectedCourse() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getRegisteredCourse);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getSelectedCourse});
}

void StudentController::updateAllCourseInfo() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getCourse);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getAllCourse});
}

void StudentController::getRelaventTaskList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getRelaventTaskList);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getRelaventTask});
}

void StudentController::getRelaventApplicationList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getRelaventAppList);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getRelaventAppList});
}

void StudentController::getSpecificAnswer() {
    DBRequest request = constructBasicRequest();

    request.setRequestService(DBRequest::RequestService::getSpecificAnswer);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::taskId, currentTaskId);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getSpecificAnswer});
}

DBRequest StudentController::constructBasicRequest() {
    DBRequest request;
    request.setUserName(username);
    request.setPassword(password);
    request.setRole(DBRequest::Role::student);
    return request;
}

void StudentController::selectCourse() {
    if (toRegisterPool.empty()) {
        emit displayError("Please select at least one course");
        return;
    }
    DBRequest request = constructBasicRequest();

    request.setRequestService(DBRequest::RequestService::registerCourse);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::courseIds,
            QJsonArray::fromStringList(toRegisterPool.toList()));

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::selectCourse});
}

void StudentController::getSpecificTask(int index) {
    QString taskId = relaventTaskIdList.at(index);
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getSpecificTask);
    request.addToRequestData(DBRequest::RequestDataKey::taskId, taskId);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getSpecificTask});
}

void StudentController::submitTask(QString answer) {
    DBRequest request = constructBasicRequest();

    request.setRequestService(DBRequest::RequestService::submitTask);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::taskId, currentTaskId);
    request.addToRequestData(DBRequest::RequestDataKey::answer, answer);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::submitTask});
}

void StudentController::prepareRegister(QString id, bool isRegistered) {
    if (isRegistered) {
        toRegisterPool.insert(id);
        qDebug() << id << "toRegister";
    } else {
        toRegisterPool.remove(id);
        qDebug() << id << "canceled";
    }
}

void StudentController::unregisterCourse(int index) {
    QString courseId = selectedCourseIdList.at(index);
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::unRegisterCourse);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::courseId, courseId);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::unregCourse});
}

void StudentController::serverResponseHandler(quint64 token) {
    // if this request is sent by us
    if (requestPool.contains(token)) {
        // get response data and remove cached request token
        DBResponse response = HttpClient::getInsatance().getResponse(
                        requestPool.value(token).first);
        RequestType type = requestPool.value(token).second;
        requestPool.remove(token);

        // process common error
        switch (response.getCommonError()) {
            case DBResponse::CommonErrorCode::internalError:
                emit displayError("Server internal error.");
                return;
            case DBResponse::CommonErrorCode::serverBusy:
                emit displayError("Server busy.");
                return;
            default:
                break;
        }

        // process result
        switch (type) {
            case RequestType::getSelectedCourse: {
                QJsonArray ids = response.getTable()
                        .value(Config::ColumnName::Course::id).toArray();
                QJsonArray names = response.getTable()
                        .value(Config::ColumnName::Course::name).toArray();

                // update selecte course list
                selectedCourseList.clear();
                selectedCourseIdList.clear();
                for (auto i : ids) {
                    selectedCourseList.append(i.toString());
                    selectedCourseIdList.append(i.toString());
                }
                for (int i = 0; i < MIN(names.size(), selectedCourseList.size()); ++i)
                    selectedCourseList[i] += QString(": ") += names.at(i).toString();
                refreshSelectedCourseModel();
                break;
            }
            case RequestType::getAllCourse: {
                namespace ColName = Config::ColumnName::Course;
                allCourses.clearCourse();
                QJsonObject table = response.getTable();
                for (int i = 0; i < table.value(ColName::id).toArray().size(); ++i) {
                    allCourses.addCourse(CourseModel::Course(
                                    table.value(ColName::id).toArray().at(i).toString(),
                                    table.value(ColName::name).toArray().at(i).toString(),
                                    table.value(ColName::span).toArray().at(i).toInt(),
                                    table.value(ColName::season).toArray().at(i).toInt(),
                                    selectedCourseIdList.contains(table.value(ColName::id).toArray().at(i).toString()),
                                    pendingCourseList.contains(table.value(ColName::id).toArray().at(i).toString())
                            ));
                }
                break;
            }
            case RequestType::getRelaventTask: {
                QJsonObject table = response.getTable();
                int tempScore;
                relaventTaskList.clear();
                relaventTaskIdList.clear();
                for (int i = 0; i < table.value(Config::TableName::task
                                + "." + Config::ColumnName::Task::id)
                        .toArray().size(); ++i) {
                    relaventTaskList.append(
                            QString("%1: %2, scores: %3")
                            .arg(table.value(Config::TableName::course + "."
                                            + Config::ColumnName::Course::name)
                                    .toArray().at(i).toString())
                            .arg(table.value(Config::TableName::task + "."
                                            + Config::ColumnName::Task::id)
                                    .toArray().at(i).toString())
                            .arg((tempScore = table.value(Config::TableName::studentTask + "."
                                                            + Config::ColumnName::StudentTask::score)
                                                    .toArray().at(i).toInt()) == -1 ? "no score" : QString::number(tempScore))
                    );
                    relaventTaskIdList.append(
                            table.value(Config::TableName::task + "."
                                    + Config::ColumnName::Task::id)
                            .toArray().at(i).toString());
                }
                refreshRelaventTaskModel();
                break;
            }
            case RequestType::getSpecificTask: {
                if (response.getServiceError() == DBResponse::ServiceErrorCode::noTask) {
                    emit displayError("Task not found.");
                    break;
                }
                QJsonObject table = response.getTable();
                currentTaskId = table.value(Config::ColumnName::Task::id).toString();
                currentTaskQuestion = table.value(Config::ColumnName::Task::question).toString();
                emit currentTaskIdChanged(currentTaskId);
                emit currentTaskQuestionChanged(currentTaskQuestion);
                // chain to get relative answer
                getSpecificAnswer();
                break;
            }
            case RequestType::selectCourse: {
                if (response.getServiceError() == DBResponse::ServiceErrorCode::courseRegisterFailed) {
                    QString list;
                    for (QJsonValue value : response.getFromResponseTable(
                                    DBResponse::ResponseDataKey::failedCourses).toArray())
                        list += QString(" ") += value.toString();
                    emit displayError(QString("Courses " + list + QString(" already registered.")));
                } else {
                    // clear to-register pool
                    toRegisterPool.clear();
                    // re-initialize
                    getSelectedCourse();
                    getRelaventTaskList();
                    getRelaventApplicationList();
                    // refresh course list to add course-selection constrains
                    updateAllCourseInfo();
                    emit displayMessage("Courses application sent successfully");
                }
                break;
            }
            case RequestType::submitTask: {
                if (response.getServiceError() == DBResponse::ServiceErrorCode::taskAlreadySubmitted) {
                    emit displayError("Task already submitted, cannot submit again");
                    break;
                }
                emit displayMessage("Task submitted successfully");
                break;
            }
            case RequestType::getRelaventAppList: {
                // update relavenet task
                QJsonObject table = response.getTable();
                namespace ColName = Config::ColumnName::StudentApplication;

                QString statusIcon;
                relaventApplicationList.clear();
                relaventApplicationIdList.clear();
                pendingCourseList.clear();
                for (int i = 0; i < table.value(ColName::id).toArray().size(); ++i) {
                    if (table.value(ColName::isApproved).toArray().at(i).isNull())
                        statusIcon = QString("\u2753");
                    else
                        statusIcon = table.value(ColName::isApproved)
                                .toArray().at(i).toBool() ? QString("\u2714") : QString("\u2718");

                    relaventApplicationList.append( statusIcon + " " + table
                            .value(ColName::statement).toArray().at(i).toString());
                    relaventApplicationIdList.append(table
                            .value(ColName::statement).toArray().at(i).toString());
                }
                // update pending course set
                for (QJsonValue courseId : response.getFromResponseTable(
                                DBResponse::ResponseDataKey::pendingCourseSet).toArray())
                    pendingCourseList.insert(courseId.toString());
                refreshRelaventApplicationModel();
                break;
            }
            case RequestType::unregCourse: {
                // this will always succeed if program reaches here
                getRelaventApplicationList();
                emit displayMessage("Unregister request sent.");
                break;
            }
            case RequestType::getSpecificAnswer: {
                // reset current answer
                currentTaskAnswer = QString("");
                isCurrentTaskSubmitted = false;
                if (response.getServiceError() == DBResponse::ServiceErrorCode::studentTaskNotFound) {
                    emit currentTaskAnswerChanged(currentTaskAnswer);
                    emit isCurrentTaskSubmittedChanged(isCurrentTaskSubmitted);
                    break;
                }
                currentTaskAnswer = response.getTable().value(Config::ColumnName::StudentTask::answer).toString();
                isCurrentTaskSubmitted = true;
                emit currentTaskAnswerChanged(currentTaskAnswer);
                emit isCurrentTaskSubmittedChanged(isCurrentTaskSubmitted);
            }
            // TODO: add more
            default:
                break;
        }
    }
}
