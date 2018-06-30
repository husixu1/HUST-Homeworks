#include "teachercontroller.h"
#include "dbrequest.h"
#include "dbresponse.h"
#include "globalconfig.h"
#include "protocolconstants.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

void TeacherController::getTeachingCourses() {
    DBRequest request = constructBasicRequest();
    // fill in the request
    request.setRequestService(DBRequest::RequestService::getTeachingCourse);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getTeachingCourse});
}

void TeacherController::getAssignedTaskList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getAssignedTaskList);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getAssignedTaskList});
}

void TeacherController::getRelaventApplicationList() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getRelaventAppList);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getRelaventAppList});
}

DBRequest TeacherController::constructBasicRequest() {
    DBRequest request;
    request.setUserName(username);
    request.setPassword(password);
    request.setRole(DBRequest::Role::teacher);
    return request;
}

TeacherController::TeacherController(QObject *parent) : QObject(parent) {
    // connect process request handler
    connect(&HttpClient::getInsatance(), SIGNAL(serverResponsed(quint64)),
            this, SLOT(serverResponseHandler(quint64)),
            static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
}

void TeacherController::registerQmlTypes() {
    qmlRegisterType<TeacherController>("DBLab3.TeacherController", 0, 1, "TeacherController");
}

void TeacherController::refreshTeachingCourseModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "teachingCourseModel", QVariant::fromValue(teachingCourseList));
}

void TeacherController::refreshAssignedTaskModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "assignedTaskModel", QVariant::fromValue(assignedTaskList));
}

void TeacherController::refreshRelaventStudentListModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "relaventStudentsModel", QVariant::fromValue(relaventStudnetList));
}

void TeacherController::refreshRelaventApplicationModel() {
    Config::qmlEngine->rootContext()->setContextProperty(
            "relaventAppModel", QVariant::fromValue(relaventApplicationList));
}

void TeacherController::exportAllCourseModel() {
    filteredCourses.setSourceModel(&allCourses);
    Config::qmlEngine->rootContext()->setContextProperty(
            "allCourseModel", &filteredCourses);
}

void TeacherController::initialize() {
    getTeachingCourses();
    getAssignedTaskList();
    getRelaventApplicationList();

    // TODO:
    refreshAssignedTaskModel();
    refreshTeachingCourseModel();
    refreshRelaventStudentListModel();
    refreshRelaventApplicationModel();
    exportAllCourseModel();
}

bool TeacherController::getIsCourseSelected(QString courseId) {
    return allCourses.getIsSelected(courseId);
}

bool TeacherController::getIsCoursePending(QString courseId) {
    return allCourses.getIsPending(courseId);
}

void TeacherController::updateAllCourseInfo() {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getCourse);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getAllCourse});
}

void TeacherController::selectCourseToTeach() {
    if (toRegisterPool.empty()) {
        emit displayError("Please select at least one course");
        return;
    }
    DBRequest request = constructBasicRequest();

    request.setRequestService(DBRequest::RequestService::selectCourseToTeach);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::courseIds,
            QJsonArray::fromStringList(toRegisterPool.toList()));

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::selectCourseToTeach});
}

void TeacherController::getStudentsForTask(int index) {
    QString taskId = assignedTaskIdList.at(index);
    getStudentsForTask(taskId);
}

void TeacherController::getStudentsForTask(QString taskId) {
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::getStudentsForTask);
    request.addToRequestData(DBRequest::RequestDataKey::taskId, taskId);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getStudentsForTask});
}

void TeacherController::getSpecificAnswer(int index) {
    QString studentId = relaventStudnetIdList.at(index);
    getSpecificAnswer(studentId);
}

void TeacherController::getSpecificAnswer(QString studentId) {
    DBRequest request = constructBasicRequest();

    request.setRequestService(DBRequest::RequestService::getSpecificAnswer);
    request.addToRequestData(DBRequest::RequestDataKey::id, studentId);
    request.addToRequestData(DBRequest::RequestDataKey::taskId, currentTaskId);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::getSpecificAnswer});
}

void TeacherController::gradeStudent(int grade) {
    DBRequest request = constructBasicRequest();

    request.setRequestService(DBRequest::RequestService::gradeStudent);
    request.addToRequestData(DBRequest::RequestDataKey::id, currentStudentId);
    request.addToRequestData(DBRequest::RequestDataKey::taskId, currentTaskId);
    request.addToRequestData(DBRequest::RequestDataKey::grade, grade);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::gradeStudent});
}

void TeacherController::addTask(int courseIndex, QString question) {
    QString courseId = teachingCourseList.at(courseIndex);
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::addTask);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::courseId, courseId);
    request.addToRequestData(DBRequest::RequestDataKey::question, question);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::addTask});
}

void TeacherController::unregisterCourse(int index) {
    QString courseId = teachingCourseIdList.at(index);
    DBRequest request = constructBasicRequest();
    request.setRequestService(DBRequest::RequestService::unRegisterCourse);
    request.addToRequestData(DBRequest::RequestDataKey::id, identity);
    request.addToRequestData(DBRequest::RequestDataKey::courseId, courseId);

    HttpClient::Token token = HttpClient::getInsatance().sendRequest(request);
    requestPool.insert(*token, {token, RequestType::unregCourse});
}

void TeacherController::serverResponseHandler(quint64 token) {
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
        }

        // process result
        switch (type) {
            case RequestType::getTeachingCourse: {
                QJsonArray ids = response.getTable()
                        .value(Config::ColumnName::Course::id).toArray();
                QJsonArray names = response.getTable()
                        .value(Config::ColumnName::Course::name).toArray();

                teachingCourseList.clear();
                teachingCourseIdList.clear();
                for (auto i : ids) {
                    teachingCourseIdList.append(i.toString());
                    teachingCourseList.append(i.toString());
                }
                for (int i = 0; i < MIN(names.size(), teachingCourseList.size()); ++i)
                    teachingCourseList[i] += QString(": ") += names.at(i).toString();
                refreshTeachingCourseModel();
                break;
            }
            case RequestType::getAssignedTaskList: {
                QJsonObject table = response.getTable();
                assignedTaskList.clear();
                assignedTaskIdList.clear();
                for (int i = 0; i < table.value(Config::TableName::task
                                + "." + Config::ColumnName::Task::id)
                        .toArray().size(); ++i) {
                    assignedTaskList.append(
                            QString("%1: %2, type: %3")
                            .arg(table.value(Config::TableName::course + "."
                                            + Config::ColumnName::Course::name)
                                    .toArray().at(i).toString())
                            .arg(table.value(Config::TableName::task + "."
                                            + Config::ColumnName::Task::id)
                                    .toArray().at(i).toString())
                            .arg(table.value(Config::TableName::task + "."
                                            + Config::ColumnName::Task::type)
                                    .toArray().at(i).toString())
                    );
                    assignedTaskIdList.append(
                            table.value(Config::TableName::task + "."
                                    + Config::ColumnName::Task::id)
                            .toArray().at(i).toString());
                }
                refreshAssignedTaskModel();
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
                                    teachingCourseIdList.contains(table.value(ColName::id).toArray().at(i).toString()),
                                    pendingCourseList.contains(table.value(ColName::id).toArray().at(i).toString())
                            ));
                }
                break;
            }
            case RequestType::selectCourseToTeach: {
                if (response.getServiceError() ==
                        DBResponse::ServiceErrorCode::courseRegisterFailed) {
                    QString list;
                    for (QJsonValue value : response.getFromResponseTable(
                                    DBResponse::ResponseDataKey::failedCourses).toArray())
                        list += QString(" ") += value.toString();
                    emit displayError(QString("Courses " + list + QString(" already registered.")));
                } else {
                    // clear to-register pool
                    toRegisterPool.clear();
                    // re-initialize
                    getTeachingCourses();
                    getAssignedTaskList();
                    getRelaventApplicationList();
                    // refresh course list to add course-selection constrains
                    updateAllCourseInfo();
                    emit displayMessage("Courses registered successfully");
                }
                break;
            }
            case RequestType::getStudentsForTask: {
                if (response.getServiceError() !=
                        DBResponse::ServiceErrorCode::noError) {
                    if (response.getServiceError() ==
                            DBResponse::ServiceErrorCode::noTask)
                        emit displayError("Task not found");
                    emit displayError("Unknown error");
                    qWarning() << response.getServiceError();
                    return;
                }

                // TODO: update&refresh student list
                relaventStudnetList.clear();
                relaventStudnetIdList.clear();

                QJsonObject table = response.getTable();
                int tempScore;
                for (int i = 0; i < table.value(Config::TableName::student
                                + "." + Config::ColumnName::Student::id)
                        .toArray().size(); ++i) {
                    tempScore = table.value(Config::TableName::studentTask
                                    + "." + Config::ColumnName::StudentTask::score)
                            .toArray().at(i).toInt();
                    relaventStudnetList.append(
                            table.value(Config::TableName::student
                                    + "." + Config::ColumnName::Student::id)
                            .toArray().at(i).toString() + ": " +
                            table.value(Config::TableName::student
                                    + "." + Config::ColumnName::Student::name)
                            .toArray().at(i).toString() +
                            (tempScore == 0 ? QString("") : QString(", score: ")
                                    + QString::number(tempScore)));
                    relaventStudnetIdList.append(
                            table.value(Config::TableName::student
                                    + "." + Config::ColumnName::Student::id)
                            .toArray().at(i).toString());
                }
                refreshRelaventStudentListModel();

                // set current task id
                currentTaskId = response.getTable().value(
                                Config::TableName::task + "." +
                                Config::ColumnName::Task::id).toString();
                emit currentTaskIdChanged(currentTaskId);

                currentTaskQuestion = response.getTable().value(
                                Config::TableName::task + "." +
                                Config::ColumnName::Task::question).toString();
                emit currentTaskQuestionChanged(currentTaskQuestion);
                break;
            }
            case RequestType::getSpecificAnswer: {
                if (response.getServiceError() !=
                        DBResponse::ServiceErrorCode::noError) {
                    if (response.getServiceError() ==
                            DBResponse::ServiceErrorCode::studentTaskNotFound)
                        emit displayError("Task-Student group not found");
                    emit displayError("Unknown error");
                    qWarning() << response.getServiceError();
                    return;
                }

                // Should use a map. this is a inefficient way, buy whatever
                int index = 0;
                for (QString id : relaventStudnetIdList) {
                    if (id == response.getTable()
                            .value(Config::ColumnName::StudentTask::studentId)
                            .toString()) {
                        break;
                    }
                    ++index;
                }
                // change current student id and info
                currentStudentId = response.getTable().
                        value(Config::ColumnName::StudentTask::studentId)
                        .toString();
                currentStudentInfo = relaventStudnetList.at(index);
                emit currentStudentInfoChanged(currentStudentInfo);

                // change current task answer
                currentTaskAnswer = response.getTable()
                        .value(Config::ColumnName::StudentTask::answer)
                        .toString();
                emit currentTaskAnswerChanged(currentTaskAnswer);
                break;
            }
            case RequestType::gradeStudent: {
                // this will always be a successful update, even student or task
                // no longer exists (this will become a successful but invalid
                // operation)
                emit displayMessage("Graded successfully");
                // chain to refresh student table and student info
                getStudentsForTask(currentTaskId);
                getSpecificAnswer(currentStudentId);
                break;
            }
            case RequestType::addTask: {
                // refresh task list
                getAssignedTaskList();
                // insert will always be successful if program runs here
                emit displayMessage(QString("Task added successfully, task id: ")
                        + response.getTable()
                        .value(Config::ColumnName::Task::id).toString());
                break;
            }
            case RequestType::getRelaventAppList: {
                // update relavenet task
                QJsonObject table = response.getTable();
                namespace ColName = Config::ColumnName::TeacherApplication;

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
            // TODO: add more
            default:
                break;
        }
    }
}

void TeacherController::prepareCourses(QString id, bool isRegistered) {
    if (isRegistered) {
        toRegisterPool.insert(id);
        qDebug() << id << "toRegister";
    } else {
        toRegisterPool.remove(id);
        qDebug() << id << "canceled";
    }
}
