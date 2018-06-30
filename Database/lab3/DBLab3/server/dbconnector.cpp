#include "dbconnector.h"
#include "globalconfig.h"
#include "SQLParser.h"
#include "servicemanager.h"
#include "httpserver.h"
#include "qhttpserver.hpp"
#include "qhttpserverresponse.hpp"
#include "finally.h"
#include "protocolconstants.h"

#include <QDebug>
#include <QSqlError>
#include <QJsonDocument>
#include <QThread>
#include <QMessageLogger>
#include <QSqlQuery>
#include <QJsonArray>
#include <QDateTime>

using Service = DBRequest::RequestService;
using Role = DBRequest::Role;
using Key = DBRequest::RequestDataKey;

// TODO: move this and related to global config maybe
static const QMap<Role, QSet<Service>> permission( {
    {
        Role::manager, {
            Service::login,
            Service::getStudents,
            Service::getTeachers,
            Service::getCourse,
            Service::getTeacherAppList,
            Service::getStudentAppList,
            Service::approveApplication,
            Service::declineApplication,
            //TODO: add
        }
    }, {
        Role::teacher, {
            Service::login,
            Service::getTeachingCourse,
            Service::getAssignedTaskList,
            Service::getCourse,
            Service::selectCourseToTeach,
            Service::getStudentsForTask,
            Service::getSpecificAnswer,
            Service::gradeStudent,
            Service::addTask,
            Service::getRelaventAppList,
            Service::unRegisterCourse,
            //TODO: add
        }
    }, {
        Role::student, {
            Service::login,
            Service::getRegisteredCourse,
            Service::getCourse,
            Service::getRelaventTaskList,
            Service::getSpecificTask,
            Service::submitTask,
            Service::registerCourse,
            Service::getRelaventAppList,
            Service::unRegisterCourse,
            Service::getSpecificAnswer,
            //TODO: add more ...
        }
    }
});

// TODO: add to this table on need
// additional requirements
static const QMap<Service, QSet<Key>> requirements ( {
    {Service::login,                {}},
    {Service::getRegisteredCourse,  {Key::id}},
    {Service::getCourse,            {}},
    {Service::getRelaventTaskList,  {Key::id}},
    {Service::getSpecificTask,      {Key::taskId}},
    {Service::submitTask,           {Key::id, Key::taskId, Key::answer}},
    {Service::registerCourse,       {Key::id, Key::courseIds}},
    {Service::getTeachingCourse,    {Key::id}},
    {Service::getAssignedTaskList,  {Key::id}},
    {Service::selectCourseToTeach,  {Key::id, Key::courseIds}},
    {Service::getStudentsForTask,   {Key::taskId}},
    {Service::getSpecificAnswer,    {Key::id, Key::taskId}},
    {Service::gradeStudent,         {Key::id, Key::taskId, Key::grade}},
    {Service::addTask,              {Key::id, Key::courseId, Key::question}},
    {Service::getStudents,          {}},
    {Service::getTeachers,          {}},
    {Service::getRelaventAppList,   {Key::id}},
    {Service::unRegisterCourse,     {Key::id, Key::courseId}},
    {Service::getTeacherAppList,    {}},
    {Service::getStudentAppList,    {}},
    {Service::approveApplication,   {Key::id, Key::appRole, Key::appIds}},
    {Service::declineApplication,   {Key::id, Key::appRole, Key::appIds}},
    // TODO: add more ...
});


DBConnector::DBConnector(QString name) : QObject(nullptr) {
    database = QSqlDatabase::addDatabase("QMYSQL", name);
    if (!database.isValid())
        qFatal("cannot load sql driver");
    database.setDatabaseName(Config::dataBaseName);

    requestCount = 0;
}

void DBConnector::startConnectionLater(
        const QString &hostName,
        const QString &userName,
        const QString &password) {
    QMetaObject::invokeMethod(this, STRINGIFY(startConnection),
            Qt::QueuedConnection,
            Q_ARG(QString, hostName),
            Q_ARG(QString, userName),
            Q_ARG(QString, password));
}

void DBConnector::stopConnectionLater() {
    QMetaObject::invokeMethod(this, STRINGIFY(stopConnection),
            Qt::QueuedConnection);
}

bool DBConnector::addToRequestQueue(DBRequest request, qhttp::server::QHttpResponse *response) {
    // return if max queued connection count is reached
    if (requestCount > Config::dataBaseMaxQueuedRequest)
        return false;

    // add to request count
    requestCountLock.lock();
    ++requestCount;
    requestCountLock.unlock();
    // append request to queue
    QMetaObject::invokeMethod(this,
            STRINGIFY(processRequest), Qt::QueuedConnection,
            Q_ARG(DBRequest, request),
            Q_ARG(qhttp::server::QHttpResponse *, response));
    return true;
}


void DBConnector::startConnection(
        const QString &hostName,
        const QString &userName,
        const QString &password) {

    if (database.hostName() == hostName &&
            database.userName() == userName &&
            database.password() == password &&
            database.isOpen()) {
        // if already connected with exactly the same information
        qInfo("database already connected");
        emit connectionStarted();
    }

    // (re)connect to database
    qInfo("(re)connecting to database");
    database.close();
    database.setUserName(userName);
    database.setPassword(password);

    // try connecting
    if (database.open()) {
        qInfo("connected to database");
        emit connectionStarted();
    } else {
        qFatal("failed connecting to database: %s",
                database.lastError().text().toStdString().c_str());
        emit connectionFailed();
    }
}

void DBConnector::stopConnection() {
    qInfo("dbconnector connection stopped");
    if (database.isOpen())
        database.close();
    emit connectionClosed();
}
/// for storage in QSet
inline uint qHash(DBRequest::RequestService key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

bool DBConnector::permissionCheck(const DBRequest &request) {
    // check if has permission
    if (permission.contains(request.getRole()) &&
            permission.value(request.getRole()).contains(request.getRequestService())) {
        return true;
    }

    // didn't pass the permission check
    return false;
}

/// for storage in QSet
inline uint qHash(DBRequest::RequestDataKey key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}
bool DBConnector::integrityCheck(const DBRequest &request) {
    // unkown service
    if (!requirements.contains(request.getRequestService()))
        return false;

    // check if each key is in the request data
    for (auto key : requirements.value(request.getRequestService()))
        if (request.getValFromRequestData(key) == QJsonValue::Undefined)
            return false;

    // passed all check
    return true;
}

DBResponse DBConnector::processLoginRequest(const DBRequest &request) {
    using Role = DBRequest::Role;
    using namespace Config;

    DBResponse processResult;

    // select table name
    QString tableName, colName, passwdColName, nameColName;
    switch (request.getRole()) {
        case Role::student:
            tableName = TableName::student;
            colName = ColumnName::Student::id;
            passwdColName = ColumnName::Student::password;
            nameColName = ColumnName::Student::name;
            break;
        case Role::teacher:
            tableName = TableName::teacher;
            colName = ColumnName::Teacher::id;
            passwdColName = ColumnName::Teacher::password;
            nameColName = ColumnName::Teacher::name;
            break;
        case Role::manager:
            tableName = TableName::manager;
            colName = ColumnName::Manager::id;
            passwdColName = ColumnName::Manager::password;
            nameColName = ColumnName::Manager::name;
            break;
        default:
            processResult.setServiceError(DBResponse::ServiceErrorCode::noRole);
            return processResult;
            break;
    }

    // construct sql statement
    QSqlQuery dbResult(database);
    dbResult.prepare(
            QString("select * from %1 where %2=:realId")
            .arg(tableName)
            .arg(colName)
    );
    dbResult.bindValue(":realId", request.getUserName());


    // no such user
    if (!dbResult.exec()) {
        processResult.setCommonError(DBResponse::CommonErrorCode::internalError);
        return processResult;
    }

    if (!dbResult.next()) {
        processResult.setServiceError(DBResponse::ServiceErrorCode::noUser);
        return processResult;
    }

    // validate password
    if (dbResult.value(passwdColName).toString() != request.getPassword()) {
        processResult.setServiceError(DBResponse::ServiceErrorCode::wrongPassword);
        return processResult;
    }

    QJsonObject table;
    // set id and name
    table.insert(colName, dbResult.value(colName).toString());
    table.insert(nameColName, dbResult.value(nameColName).toString());
    processResult.setTable(table);
    return processResult;
}

DBResponse DBConnector::processGetRegisteredCourse(const DBRequest &request) {
    using Key = DBRequest::RequestDataKey;
    DBResponse result;

    QSqlQuery dbResult(database);
    dbResult.prepare( QString("select * from %1 "
                    "inner join %2 "
                    "on %1.%3=%2.%4 "
                    "where %5=:id")
            .arg(Config::TableName::studentCourse)
            .arg(Config::TableName::course)
            .arg(Config::ColumnName::StudentCourse::courseId)
            .arg(Config::ColumnName::Course::id)
            .arg(Config::ColumnName::StudentCourse::studentId));
    dbResult.bindValue(":id", request.getValFromRequestData(Key::id).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    qDebug() << dbResult.executedQuery();

    // process table
    QJsonArray idValues;
    QJsonArray nameValues;
    while (dbResult.next()) {
        idValues.append(dbResult.value(Config::ColumnName::Course::id).toString());
        nameValues.append(dbResult.value(Config::ColumnName::Course::name).toString());
    }
    QJsonObject table;
    table.insert (Config::ColumnName::Course::id, idValues);
    table.insert (Config::ColumnName::Course::name, nameValues);
    result.setTable(table);

    return result;
}

DBResponse DBConnector::processGetCourse(const DBRequest &request) {
    request.getRole(); // just to remove a warning
    DBResponse result;
    QString sql = QString("select * from %1").arg(Config::TableName::course);
    QSqlQuery dbResult = database.exec(sql);

    if (!dbResult.isActive()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    namespace ColName = Config::ColumnName::Course;
    QJsonArray courseId;
    QJsonArray courseName;
    QJsonArray span;
    QJsonArray season;
    while (dbResult.next()) {
        courseId.append(dbResult.value(ColName::id).toString());
        courseName.append(dbResult.value(ColName::name).toString());
        span.append(dbResult.value(ColName::span).toInt());
        season.append(dbResult.value(ColName::season).toInt());
    }
    QJsonObject table;
    table.insert(ColName::id, courseId);
    table.insert(ColName::name, courseName);
    table.insert(ColName::span, span);
    table.insert(ColName::season, season);
    result.setTable(table);

    return result;
}

DBResponse DBConnector::processGetRelaventTaskList(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    qDebug() << "prepare" << dbResult.prepare(QString("select * from "
                            "(select %1.%2, %3.%4, %5.%6 as sid "
                            "from (%1 "
                            "inner join %3 on %1.%7=%3.%8 "
                            "inner join %5 on %5.%9=%1.%7) "
                            "where %5.%6=:stuId) as base "
                            "left join %10 "
                            "on (base.sid=%10.%11 and base.%2=%10.%12)")
                    .arg(Config::TableName::task)                           //1
                    .arg(Config::ColumnName::Task::id)                      //2
                    .arg(Config::TableName::course)                         //3
                    .arg(Config::ColumnName::Course::name)                  //4
                    .arg(Config::TableName::studentCourse)                  //5
                    .arg(Config::ColumnName::StudentCourse::studentId)      //6
                    .arg(Config::ColumnName::Task::courseId)                //7
                    .arg(Config::ColumnName::Course::id)                    //8
                    .arg(Config::ColumnName::StudentCourse::courseId)       //9
                    .arg(Config::TableName::studentTask)                    //10
                    .arg(Config::ColumnName::StudentTask::studentId)        //11
                    .arg(Config::ColumnName::StudentTask::taskId)           //12
            );
    dbResult.bindValue(":stuId", request.getValFromRequestData(Key::id).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    qDebug() << dbResult.lastQuery();

    QJsonArray taskId;
    QJsonArray courseName;
    QJsonArray score;
    while (dbResult.next()) {
        taskId.append(dbResult.value("base." + Config::ColumnName::Task::id).toString());
        courseName.append(dbResult.value("base." + Config::ColumnName::Course::name).toString());
        score.append(dbResult.value(Config::ColumnName::StudentTask::score).isNull() ? -1 :
                dbResult.value(Config::ColumnName::StudentTask::score).toInt());
    }
    QJsonObject table;
    table.insert(Config::TableName::task
            + "." + Config::ColumnName::Task::id, taskId);
    table.insert(Config::TableName::course
            + "." + Config::ColumnName::Course::name, courseName);
    table.insert(Config::TableName::studentTask
            + "." + Config::ColumnName::StudentTask::score, score);

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processGetSpecificTask(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare( QString("select * from %1 where %2=:taskId")
            .arg(Config::TableName::task)
            .arg(Config::ColumnName::Task::id));
    dbResult.bindValue(":taskId", request.getValFromRequestData(Key::taskId).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    qDebug() << dbResult.lastQuery();

    if (!dbResult.next()) {
        result.setServiceError(DBResponse::ServiceErrorCode::noTask);
        return result;
    }

    namespace ColName = Config::ColumnName::Task;
    QJsonObject table;
    table.insert(ColName::id,       dbResult.value(ColName::id       ).toString());
    table.insert(ColName::courseId, dbResult.value(ColName::courseId ).toString());
    table.insert(ColName::teacherId, dbResult.value(ColName::teacherId).toString());
    table.insert(ColName::question, dbResult.value(ColName::question ).toString());
    table.insert(ColName::startTime, dbResult.value(ColName::startTime).toDateTime().toSecsSinceEpoch());
    table.insert(ColName::endTime,  dbResult.value(ColName::endTime  ).toDateTime().toSecsSinceEpoch());

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processSubmitTask(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare( QString("insert into %1 values "
                    "(:sId, :tId, :ans, null, :time)")
            .arg(Config::TableName::studentTask));     // table name
    dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());     // student id
    dbResult.bindValue(":tId", request.getValFromRequestData(Key::taskId).toString());     // task id
    dbResult.bindValue(":ans", request.getValFromRequestData(Key::answer).toString());     // task answer
    dbResult.bindValue(":time", QDateTime::currentDateTime());     // task answer

    if (!dbResult.exec()) {
        // duplicate entry
        if (dbResult.lastError().nativeErrorCode() == "1062") {
            result.setServiceError(DBResponse::ServiceErrorCode::taskAlreadySubmitted);
            return result;
        }
        qDebug() << dbResult.lastError().nativeErrorCode();
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    return result;

}

DBResponse DBConnector::studentApplicationProcess(
        const DBRequest &request, QString type) {
    DBResponse result;
    QSqlQuery dbResult(database);

    auto rollBack = [&]() {
        // try to roll back if cannot commit
        if (!database.rollback())
            result.setCommonError(DBResponse::CommonErrorCode::internalError);
        result.setCommonError(DBResponse::CommonErrorCode::transactionFailed);
    };

    // serialize the orignial statement
    QString statement = constructApplicationJson(type, request);

    // start transaction
    if (!database.transaction()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    dbResult.prepare(QString("select %1 from %2 order by %1 desc limit 1")
            .arg(Config::ColumnName::StudentApplication::id)
            .arg(Config::TableName::studentApplication));

    if (!dbResult.exec()) {
        rollBack();
        return result;
    }
    // get a new index
    QString newId = calcNewInex(QString("A"), dbResult,
                    Config::ColumnName::StudentApplication::id);

    dbResult.prepare(QString("insert into %1 values (:aId, :stmt, :sId, null, null)")
            .arg(Config::TableName::studentApplication));
    dbResult.bindValue(":aId", newId);
    dbResult.bindValue(":stmt", statement);
    dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());

    if (!dbResult.exec()) {
        rollBack();
        return result;
    }

    dbResult.finish();
    if (!database.commit()) {
        rollBack();
        return result;
    }

    return result;

}

DBResponse DBConnector::teacherApplicationProcess(const DBRequest &request, QString type) {
    DBResponse result;
    QSqlQuery dbResult(database);

    auto rollBack = [&]() {
        // try to roll back if cannot commit
        if (!database.rollback())
            result.setCommonError(DBResponse::CommonErrorCode::internalError);
        result.setCommonError(DBResponse::CommonErrorCode::transactionFailed);
    };

    // serialize the orignial statement
    QString statement = constructApplicationJson(type, request);

    // start transaction
    if (!database.transaction()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    dbResult.prepare(QString("select %1 from %2 order by %1 desc limit 1")
            .arg(Config::ColumnName::TeacherApplication::id)
            .arg(Config::TableName::teacherApplication));

    if (!dbResult.exec()) {
        rollBack();
        return result;
    }
    // get a new index
    QString newId = calcNewInex(QString("A"), dbResult,
                    Config::ColumnName::TeacherApplication::id);

    dbResult.prepare(QString("insert into %1 values (:aId, :stmt, :sId, null, null)")
            .arg(Config::TableName::teacherApplication));
    dbResult.bindValue(":aId", newId);
    dbResult.bindValue(":stmt", statement);
    dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());

    if (!dbResult.exec()) {
        rollBack();
        return result;
    }

    dbResult.finish();
    if (!database.commit()) {
        rollBack();
        return result;
    }

    return result;
}

DBResponse DBConnector::processGetTeachingCourse(const DBRequest &request) {
    using Key = DBRequest::RequestDataKey;
    DBResponse result;

    QSqlQuery dbResult(database);
    dbResult.prepare( QString("select * from %1 "
                    "inner join %2 "
                    "on %1.%3=%2.%4 "
                    "where %5=:id")
            .arg(Config::TableName::teacherCourse)
            .arg(Config::TableName::course)
            .arg(Config::ColumnName::TeacherCourse::courseId)
            .arg(Config::ColumnName::Course::id)
            .arg(Config::ColumnName::TeacherCourse::teacherId));
    dbResult.bindValue(":id", request.getValFromRequestData(Key::id).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    qDebug() << dbResult.executedQuery();

    // process table
    QJsonArray idValues;
    QJsonArray nameValues;
    while (dbResult.next()) {
        idValues.append(dbResult.value(Config::ColumnName::Course::id).toString());
        nameValues.append(dbResult.value(Config::ColumnName::Course::name).toString());
    }
    QJsonObject table;
    table.insert (Config::ColumnName::Course::id, idValues);
    table.insert (Config::ColumnName::Course::name, nameValues);
    result.setTable(table);

    return result;

}

DBResponse DBConnector::processGetAssignedTaskList(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    qDebug() << "prepare" << dbResult.prepare(QString("select * from "
                            "%1 inner join %2 "
                            "on %1.%3=%2.%4 "
                            "where %1.%5=:id")
                    .arg(Config::TableName::task)
                    .arg(Config::TableName::course)
                    .arg(Config::ColumnName::Task::courseId)
                    .arg(Config::ColumnName::Course::id)
                    .arg(Config::ColumnName::Task::teacherId));
    dbResult.bindValue(":id", request.getValFromRequestData(Key::id).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    qDebug() << dbResult.lastQuery();

    QJsonArray taskId;
    QJsonArray taskType;
    QJsonArray courseName;
    while (dbResult.next()) {
        taskId.append(dbResult.value(Config::TableName::task
                        + "." + Config::ColumnName::Task::id).toString());
        taskType.append(dbResult.value(Config::TableName::task
                        + "." + Config::ColumnName::Task::type).toString());
        courseName.append(dbResult.value(Config::TableName::course
                        + "." + Config::ColumnName::Course::name).toString());
    }
    QJsonObject table;
    table.insert(Config::TableName::task
            + "." + Config::ColumnName::Task::id, taskId);
    table.insert(Config::TableName::task
            + "." + Config::ColumnName::Task::type, taskType);
    table.insert(Config::TableName::course
            + "." + Config::ColumnName::Course::name, courseName);

    result.setTable(table);
    return result;

}

DBResponse DBConnector::processSelectCourseToTeach(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare(
            QString("insert into %1 values (:tId, :cId)")
            .arg(Config::TableName::teacherCourse));

    // failed courses
    QJsonArray failList;
    for (QJsonValue courseId : request.getValFromRequestData(Key::courseIds).toArray()) {
        dbResult.bindValue(":tId", request.getValFromRequestData(Key::id).toString());
        dbResult.bindValue(":cId", courseId.toString());
        if (!dbResult.exec())
            failList.append(courseId.toString());
    }

    if (failList.size()) {
        result.addToResponseTable(DBResponse::ResponseDataKey::failedCourses, failList);
        result.setServiceError(DBResponse::ServiceErrorCode::courseRegisterFailed);
        return result;
    }

    return result;

}

DBResponse DBConnector::processStudentSelectCourse(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare(
            QString("insert into %1 values (:sId, :cId)")
            .arg(Config::TableName::studentCourse));

    // failed courses
    QJsonArray failList;
    for (QJsonValue courseId : request.getValFromRequestData(Key::courseIds).toArray()) {
        dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());
        dbResult.bindValue(":cId", courseId.toString());
        if (!dbResult.exec())
            failList.append(courseId.toString());
    }

    if (failList.size()) {
        result.addToResponseTable(DBResponse::ResponseDataKey::failedCourses, failList);
        result.setServiceError(DBResponse::ServiceErrorCode::courseRegisterFailed);
        return result;
    }
    return result;
}

DBResponse DBConnector::processStudentUnregisterCourse(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare(QString("delete from %1 where (%2, %3)=(:sId, :cId)")
            .arg(Config::TableName::studentCourse)
            .arg(Config::ColumnName::StudentCourse::studentId)
            .arg(Config::ColumnName::StudentCourse::courseId));

    dbResult.bindValue(":sId", request.getValFromRequestData(
                    DBRequest::RequestDataKey::id));
    dbResult.bindValue(":cId", request.getValFromRequestData(
                    DBRequest::RequestDataKey::courseId));

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    return result;
}

DBResponse DBConnector::processTeacherUnregisterCourse(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare(QString("delete from %1 where (%2, %3)=(:tId, :cId)")
            .arg(Config::TableName::teacherCourse)
            .arg(Config::ColumnName::TeacherCourse::teacherId)
            .arg(Config::ColumnName::TeacherCourse::courseId));

    dbResult.bindValue(":tId", request.getValFromRequestData(
                    DBRequest::RequestDataKey::id));
    dbResult.bindValue(":cId", request.getValFromRequestData(
                    DBRequest::RequestDataKey::courseId));

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    return result;

}

DBResponse DBConnector::processGetStudentsForTask(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    // get task question
    dbResult.prepare(QString("select %1 from %2 where %3=:tId")
            .arg(Config::ColumnName::Task::question)
            .arg(Config::TableName::task)
            .arg(Config::ColumnName::Task::id));
    dbResult.bindValue(":tId", request.getValFromRequestData(Key::taskId).toString());
    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    if (!dbResult.next()) {
        result.setServiceError(DBResponse::ServiceErrorCode::noTask);
        return result;
    }
    QJsonObject table;
    table.insert(Config::TableName::task + "." + Config::ColumnName::Task::question,
            dbResult.value(Config::ColumnName::Task::question).toString());

    // get stduents list
    dbResult.prepare(QString("select %1.%2, %1.%3, %4.%7 from "
                    "%1 inner join %4 "
                    "on %1.%2=%4.%5 "
                    "where %4.%6=:tId")
            .arg(Config::TableName::student)
            .arg(Config::ColumnName::Student::id)
            .arg(Config::ColumnName::Student::name)
            .arg(Config::TableName::studentTask)
            .arg(Config::ColumnName::StudentTask::studentId)
            .arg(Config::ColumnName::StudentTask::taskId)
            .arg(Config::ColumnName::StudentTask::score)
    );
    dbResult.bindValue(":tId", request.getValFromRequestData(Key::taskId).toString());
    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    QJsonArray ids, names, scores;
    while (dbResult.next()) {
        ids.append(dbResult.value(Config::ColumnName::Student::id).toString());
        names.append(dbResult.value(Config::ColumnName::Student::name).toString());
        QVariant score = dbResult.value(Config::ColumnName::StudentTask::score);
        scores.append(score.isNull() ? -1 : score.toInt());
    }

    table.insert(Config::TableName::student
            + "." + Config::ColumnName::Student::id, ids);
    table.insert(Config::TableName::student
            + "." + Config::ColumnName::Student::name, names);
    table.insert(Config::TableName::studentTask
            + "." + Config::ColumnName::StudentTask::score, scores);

    // echo task id for the client to use
    table.insert(Config::TableName::task + "." + Config::ColumnName::Task::id,
            request.getValFromRequestData(Key::taskId).toString());

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processGetSpecificAnswer(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    namespace ColName = Config::ColumnName::StudentTask;

    dbResult.prepare(QString("select * from %1 where (%2, %3)=(:sId, :tId)")
            .arg(Config::TableName::studentTask)
            .arg(ColName::studentId)
            .arg(ColName::taskId));
    dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());
    dbResult.bindValue(":tId", request.getValFromRequestData(Key::taskId).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    if (!dbResult.next()) {
        result.setServiceError(DBResponse::ServiceErrorCode::studentTaskNotFound);
        return result;
    }

    QJsonObject table;
    table.insert(ColName::studentId, dbResult.value(ColName::studentId).toString());
    table.insert(ColName::answer, dbResult.value(ColName::answer).toString());
    table.insert(ColName::score, dbResult.value(ColName::score).toString());
    table.insert(ColName::finishTime, dbResult.value(ColName::finishTime).toDateTime().toSecsSinceEpoch());

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processGradeStudent(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);

    dbResult.prepare(QString("update %1 "
                    "set %2=:grade "
                    "where (%4, %5)=(:sId, :tId)")
            .arg(Config::TableName::studentTask)
            .arg(Config::ColumnName::StudentTask::score)
            .arg(Config::ColumnName::StudentTask::studentId)
            .arg(Config::ColumnName::StudentTask::taskId));
    dbResult.bindValue(":grade", request.getValFromRequestData(Key::grade).toInt());
    dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());
    dbResult.bindValue(":tId", request.getValFromRequestData(Key::taskId).toString());

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }
    return result;
}

DBResponse DBConnector::processAddTask(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);

    auto rollBack = [&]() {
        // try to roll back if cannot commit
        if (!database.rollback())
            result.setCommonError(DBResponse::CommonErrorCode::internalError);
        result.setCommonError(DBResponse::CommonErrorCode::transactionFailed);
    };

    if (!database.transaction()) {
        // if cannot start a transaction
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    // get current max id first
    dbResult.prepare(QString("select %1 from %2 order by %1 desc limit 1")
            .arg(Config::ColumnName::Task::id)
            .arg(Config::TableName::task));
    if (!dbResult.exec()) {
        rollBack();
        return result;
    }
    QString newId;
    newId = calcNewInex(QString("W"), dbResult,
                    Config::ColumnName::Task::id);

    // append task
    dbResult.prepare(QString("insert into %1 values "
                    "(:id, :cId, :tId, :q, :start, :end, :type)")
            .arg(Config::TableName::task));
    dbResult.bindValue(":id", newId);
    dbResult.bindValue(":cId",
            request.getValFromRequestData(Key::courseId).toString());
    dbResult.bindValue(":tId",
            request.getValFromRequestData(Key::id).toString());
    dbResult.bindValue(":q",
            request.getValFromRequestData(Key::question).toString());
    // TODO: change this to modifiable
    dbResult.bindValue(":start", QDateTime::currentDateTime());
    dbResult.bindValue(":end", QDateTime::currentDateTime().addMonths(1));
    dbResult.bindValue(":type", QString("task"));

    if (!dbResult.exec()) {
        rollBack();
        return result;
    }

    // commit changes
    dbResult.finish();
    if (!database.commit()) {
        rollBack();
        return result;
    }

    // return new task id
    QJsonObject table;
    table.insert(Config::ColumnName::Task::id, newId);
    result.setTable(table);
    return result;
}

DBResponse DBConnector::processGetStudents(const DBRequest &) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare(QString("select * from %1")
            .arg(Config::TableName::student));

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    QJsonArray ids;
    QJsonArray names;
    QJsonArray usernames;
    while (dbResult.next()) {
        ids.append(dbResult.value(Config::ColumnName::Student::id).toString());
        names.append(dbResult.value(Config::ColumnName::Student::name).toString());
        usernames.append(dbResult.value(Config::ColumnName::Student::username).toString());
    }
    QJsonObject table;
    table.insert(Config::ColumnName::Student::id, ids);
    table.insert(Config::ColumnName::Student::name, names);
    table.insert(Config::ColumnName::Student::username, usernames);

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processGetTeachers(const DBRequest &) {
    DBResponse result;
    QSqlQuery dbResult(database);
    dbResult.prepare(QString("select * from %1")
            .arg(Config::TableName::teacher));

    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    QJsonArray ids;
    QJsonArray names;
    QJsonArray usernames;
    while (dbResult.next()) {
        ids.append(dbResult.value(Config::ColumnName::Teacher::id).toString());
        names.append(dbResult.value(Config::ColumnName::Teacher::name).toString());
        usernames.append(dbResult.value(Config::ColumnName::Teacher::username).toString());
    }
    QJsonObject table;
    table.insert(Config::ColumnName::Teacher::id, ids);
    table.insert(Config::ColumnName::Teacher::name, names);
    table.insert(Config::ColumnName::Teacher::username, usernames);

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processApproveApplication(const DBRequest &request) {
    // get app from db and parse it
    DBResponse result;
    QSqlQuery dbResult(database);

    DBRequest::Role role;
    if (request.getValFromRequestData(DBRequest::RequestDataKey::appRole) ==
            Config::ApplicationRole::student) {
        role = DBRequest::Role::student;
    } else if (request.getValFromRequestData(DBRequest::RequestDataKey::appRole) ==
            Config::ApplicationRole::teacher) {
        role = DBRequest::Role::teacher;
    } else {
        result.setServiceError(DBResponse::ServiceErrorCode::noRole);
        return result;
    }


    for (QJsonValue appId : request
            .getValFromRequestData(DBRequest::RequestDataKey::appIds).toArray()) {

        // get the original statement
        switch (role) {
            case DBRequest::Role::student:
                dbResult.prepare(QString("select * from %1 where %2=:aId")
                        .arg(Config::TableName::studentApplication)
                        .arg(Config::ColumnName::StudentApplication::id));
                break;
            case DBRequest::Role::teacher:
                dbResult.prepare(QString("select * from %1 where %2=:aId")
                        .arg(Config::TableName::teacherApplication)
                        .arg(Config::ColumnName::TeacherApplication::id));
                break;
            default:
                break;
        }
        dbResult.bindValue(":aId", appId.toString());
        if (!dbResult.exec()) {
            result.setCommonError(DBResponse::CommonErrorCode::partialInternalError);
            continue;
        }
        if (!dbResult.next()) {
            result.setServiceError(DBResponse::ServiceErrorCode::partialAppNotFound);
            continue;
        }
        QString statement;
        switch (role) {
            case DBRequest::Role::student:
                statement = dbResult.value(Config::ColumnName::StudentApplication::statement).toString();
                break;
            case DBRequest::Role::teacher:
                statement = dbResult.value(Config::ColumnName::TeacherApplication::statement).toString();
                break;
            default:
                break;
        }

        // convert original statement to original request
        auto app = restoreApplicationJson(statement);
        DBResponse response;
        if (app.first == Config::ApplicationType::studentRegisterCourse) {
            response = processStudentSelectCourse(app.second);
        } else if (app.first == Config::ApplicationType::studentUnregisterCourse) {
            response = processStudentUnregisterCourse(app.second);
        } else if (app.first == Config::ApplicationType::teacherRegisterCourse) {
            response = processSelectCourseToTeach(app.second);
        } else if (app.first == Config::ApplicationType::teacherUnregisterCourse) {
            response = processTeacherUnregisterCourse(app.second);
        } else {
            //TODO: processTeacherAddCourse
            response = DBResponse();
        }

        // TODO: should have set application to an error state instead of
        // declined state, but whatever
        bool isAccepct = false;
        if (response.getCommonError() == DBResponse::CommonErrorCode::noError &&
                response.getServiceError() == DBResponse::ServiceErrorCode::noError)
            isAccepct = true;

        // if executed successfully, set applicatoin to accepted state
        switch (role) {
            case DBRequest::Role::student:
                dbResult.prepare(QString("update %1 set %2=:mId, %3=:acc where %4=:aId")
                        .arg(Config::TableName::studentApplication)
                        .arg(Config::ColumnName::StudentApplication::managerId)
                        .arg(Config::ColumnName::StudentApplication::isApproved)
                        .arg(Config::ColumnName::StudentApplication::id));
                break;
            case DBRequest::Role::teacher:
                dbResult.prepare(QString("update %1 set %2=:mId, %3=:acc where %4=:aId")
                        .arg(Config::TableName::teacherApplication)
                        .arg(Config::ColumnName::TeacherApplication::managerId)
                        .arg(Config::ColumnName::TeacherApplication::isApproved)
                        .arg(Config::ColumnName::TeacherApplication::id));
                break;
            default:
                break;
        }
        dbResult.bindValue(":mId", request.getValFromRequestData(Key::id));
        dbResult.bindValue(":acc", isAccepct);
        dbResult.bindValue(":aId", appId.toString());

        if (!dbResult.exec()) {
            result.setCommonError(DBResponse::CommonErrorCode::partialInternalError);
            continue;
        }
    }
    return result;
}

DBResponse DBConnector::processDeclineApplication(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);

    DBRequest::Role role;
    if (request.getValFromRequestData(DBRequest::RequestDataKey::appRole) ==
            Config::ApplicationRole::student) {
        role = DBRequest::Role::student;
    } else if (request.getValFromRequestData(DBRequest::RequestDataKey::appRole) ==
            Config::ApplicationRole::teacher) {
        role = DBRequest::Role::teacher;
    } else {
        result.setServiceError(DBResponse::ServiceErrorCode::noRole);
        return result;
    }

    for (QJsonValue appId : request
            .getValFromRequestData(DBRequest::RequestDataKey::appIds).toArray()) {

        // if executed successfully, set applicatoin to accepted state
        switch (role) {
            case DBRequest::Role::student:
                dbResult.prepare(QString("update %1 set %2=:mId, %3=:acc where %4=:aId")
                        .arg(Config::TableName::studentApplication)
                        .arg(Config::ColumnName::StudentApplication::managerId)
                        .arg(Config::ColumnName::StudentApplication::isApproved)
                        .arg(Config::ColumnName::StudentApplication::id));
                break;
            case DBRequest::Role::teacher:
                dbResult.prepare(QString("update %1 set %2=:mId, %3=:acc where %4=:aId")
                        .arg(Config::TableName::teacherApplication)
                        .arg(Config::ColumnName::TeacherApplication::managerId)
                        .arg(Config::ColumnName::TeacherApplication::isApproved)
                        .arg(Config::ColumnName::TeacherApplication::id));
            default:
                break;
        }
        dbResult.bindValue(":mId", request.getValFromRequestData(Key::id));
        dbResult.bindValue(":acc", false);
        dbResult.bindValue(":aId", appId.toString());

        if (!dbResult.exec()) {
            result.setCommonError(DBResponse::CommonErrorCode::partialInternalError);
            continue;
        }
    }
    return result;
}

DBResponse DBConnector::processGetRelaventAppList(const DBRequest &request) {
    DBResponse result;
    QSqlQuery dbResult(database);
    using Role = DBRequest::Role;
    Role role = request.getRole();

    // prepare the request according to role (teacher/student)
    if (role == Role::student) {
        dbResult.prepare(QString("select * from %1 where %2=:sId")
                .arg(Config::TableName::studentApplication)
                .arg(Config::ColumnName::StudentApplication::studentId));
        dbResult.bindValue(":sId", request.getValFromRequestData(Key::id).toString());
    } else if (role == Role::teacher) {
        dbResult.prepare(QString("select * from %1 where %2=:tId")
                .arg(Config::TableName::teacherApplication)
                .arg(Config::ColumnName::TeacherApplication::teacherId));
        dbResult.bindValue(":tId", request.getValFromRequestData(Key::id).toString());
    }

    // execute db query
    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    QJsonArray ids;
    QJsonArray statements;
    QJsonArray approvedList;
    QSet<QString> pendingCourseSet;

    // translate application to human readable string
    while (dbResult.next()) {
        // get the human-readable string of the applicatoin
        QString represent = getRepresentsOfApplication(dbResult, role, pendingCourseSet);

        if (role == Role::student) {
            ids.append(dbResult.value(Config::ColumnName::StudentApplication::id).toString());
            statements.append(represent);
            if (dbResult.value(Config::ColumnName::StudentApplication::isApproved).isNull())
                approvedList.append(QJsonValue::Null);
            else
                approvedList.append(dbResult
                        .value(Config::ColumnName::StudentApplication::isApproved).toBool());
        } else if (role == Role::teacher) {
            ids.append(dbResult.value(Config::ColumnName::TeacherApplication::id).toString());
            statements.append(represent);
            if (dbResult.value(Config::ColumnName::TeacherApplication::isApproved).isNull())
                approvedList.append(QJsonValue::Null);
            else
                approvedList.append(dbResult
                        .value(Config::ColumnName::TeacherApplication::isApproved).toBool());
        }
    }

    QJsonArray pendingCourseList;
    for (QString courseId : pendingCourseSet)
        pendingCourseList.append(courseId);

    QJsonObject table;
    if (role == Role::student) {
        table.insert(Config::ColumnName::StudentApplication::id, ids);
        table.insert(Config::ColumnName::StudentApplication::statement, statements);
        table.insert(Config::ColumnName::StudentApplication::isApproved, approvedList);
    } else if (role == Role::teacher) {
        table.insert(Config::ColumnName::TeacherApplication::id, ids);
        table.insert(Config::ColumnName::TeacherApplication::statement, statements);
        table.insert(Config::ColumnName::TeacherApplication::isApproved, approvedList);
    }
    result.setTable(table);
    result.addToResponseTable(DBResponse::ResponseDataKey::pendingCourseSet, pendingCourseList);

    return result;
}

DBResponse DBConnector::processGetTeacherAppList(const DBRequest &) {
    DBResponse result;
    QSqlQuery dbResult(database);

    // only get unapproved data
    dbResult.prepare(QString("select * from %1 where %2 is null")
            .arg(Config::TableName::teacherApplication)
            .arg(Config::ColumnName::TeacherApplication::isApproved));
    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    QJsonArray ids;
    QJsonArray represents;
    QSet<QString> pendingCourses;
    while (dbResult.next()) {
        represents.append(getRepresentsOfApplication(
                        dbResult, DBRequest::Role::teacher, pendingCourses));
        ids.append(dbResult.value(Config::ColumnName::TeacherApplication::id).toString());
    }

    QJsonObject table;
    table.insert(Config::ColumnName::TeacherApplication::id, ids);
    table.insert(Config::ColumnName::TeacherApplication::statement, represents);

    result.setTable(table);
    return result;
}

DBResponse DBConnector::processGetStudentAppList(const DBRequest &) {
    DBResponse result;
    QSqlQuery dbResult(database);

    // only get unapproved data
    dbResult.prepare(QString("select * from %1 where %2 is null")
            .arg(Config::TableName::studentApplication)
            .arg(Config::ColumnName::StudentApplication::isApproved));
    if (!dbResult.exec()) {
        result.setCommonError(DBResponse::CommonErrorCode::internalError);
        return result;
    }

    QJsonArray ids;
    QJsonArray represents;
    QSet<QString> pendingCourses;
    while (dbResult.next()) {
        represents.append(getRepresentsOfApplication(
                        dbResult, DBRequest::Role::student, pendingCourses));
        ids.append(dbResult.value(Config::ColumnName::StudentApplication::id).toString());
    }

    QJsonObject table;
    table.insert(Config::ColumnName::StudentApplication::id, ids);
    table.insert(Config::ColumnName::StudentApplication::statement, represents);

    result.setTable(table);
    return result;
}

QString DBConnector::calcNewInex(QString prefix, QSqlQuery &dbResult, QString indexColName) {
    if (!dbResult.next())
        return prefix + QString::number(QDate::currentDate().year())
                + QString("00001");
    else
        return prefix + QString::number(
                        dbResult.value(indexColName)
                        .toString().mid(1).toInt() + 1);
}

QString DBConnector::constructApplicationJson(QString type, const DBRequest &request) {
    QJsonObject object = request.toJson();
    object.insert("__type", type);
    return QJsonDocument(object).toJson();
}

QPair<QString, DBRequest> DBConnector::restoreApplicationJson(QString content) {
    QJsonObject object = QJsonDocument::fromJson(content.toUtf8()).object();
    QString type = object.value("__type").toString();
    object.remove("__type");
    return {type, DBRequest::fromJson(object)};
}

QString DBConnector::getRepresentsOfApplication(QSqlQuery &dbResult,
        DBRequest::Role role, QSet<QString> &pendingCourseSet) {
    using Role = DBRequest::Role;
    // retrive original request
    QString statement;
    if (role == Role::student) {
        statement = dbResult
                .value(Config::ColumnName::StudentApplication::statement).toString();
    } else if (role == Role::teacher) {
        statement = dbResult
                .value(Config::ColumnName::TeacherApplication::statement).toString();
    }
    auto app = restoreApplicationJson(statement);
    DBRequest originalRequest = app.second;

    // construct human readable string according to type (type implies role)
    QString represent = dbResult
            .value(Config::ColumnName::StudentApplication::id).toString();
    if (app.first == Config::ApplicationType::studentRegisterCourse) {
        represent += QString(" Register");
        QJsonArray courses = originalRequest.getValFromRequestData(Key::courseIds).toArray();
        for (auto courseId : courses)
            represent += QString(" ") += courseId.toString();

        // if this request is neither approved nor declined, it's a pending course
        // the client should lock these courses to prevent being selected again
        if (dbResult.value(Config::ColumnName::StudentApplication::isApproved).isNull())
            for (auto courseId : courses)
                pendingCourseSet.insert(courseId.toString());
    } else if (app.first == Config::ApplicationType::teacherRegisterCourse) {
        represent += QString(" Register");
        QJsonArray courses = originalRequest.getValFromRequestData(Key::courseIds).toArray();
        for (auto courseId : courses)
            represent += QString(" ") += courseId.toString();

        // if this request is neither approved nor declined, it's a pending course
        // the client should lock these courses to prevent being selected again
        if (dbResult.value(Config::ColumnName::TeacherApplication::isApproved).isNull())
            for (auto courseId : courses)
                pendingCourseSet.insert(courseId.toString());
    } else if (app.first == Config::ApplicationType::studentUnregisterCourse ||
            app.first == Config::ApplicationType::teacherUnregisterCourse) {
        represent += QString(" Unregister ");
        represent += originalRequest.getValFromRequestData(Key::courseId).toString();
    } else if (app.first == Config::ApplicationType::teacherAddCourse) {
        represent += QString(" Create ");
        represent += originalRequest.getValFromRequestData(Key::courseId).toString();
    } else {
        represent += QString(" Unknown application.");
    }
    return represent;
}

void DBConnector::processRequest(DBRequest request, qhttp::server::QHttpResponse *response) {
    // auto-decrease
    Finally([&]() {
        qDebug("request count decreased");
        // decrease request count for a finished request
        requestCountLock.lock();
        --requestCount;
        requestCountLock.unlock();
    });

    // sanity check
    if (!permissionCheck(request)) {
        qInfo() << "request " << " didn't pass permission check";
        // TODO: return a fault string
        DBResponse processResult;
        processResult.setServiceError(
                DBResponse::ServiceErrorCode::permissionCheckFailed);
        ServiceManager::getInstance().getHttpServer()->respondToRequest(
                response, processResult);
        return;
    }

    // integrity check
    if (!integrityCheck(request)) {
        qInfo() << "request " << " didn't pass sanity check";
        // TODO: return a fault string
        DBResponse processResult;
        processResult.setServiceError(
                DBResponse::ServiceErrorCode::integrityCheckFailed);
        ServiceManager::getInstance().getHttpServer()->respondToRequest(
                response, processResult);
        return;
    }

    // TODO: password check
    qDebug().noquote() << QJsonDocument(request.toJson()).toJson();

    // process the request
    DBResponse processResult;
    using Service = DBRequest::RequestService;
    switch (request.getRequestService()) {
        case Service::login:
            processResult = processLoginRequest(request);
            break;
        case Service::getRegisteredCourse:
            processResult = processGetRegisteredCourse(request);
            break;
        case Service::getCourse:
            processResult = processGetCourse(request);
            break;
        case Service::getRelaventTaskList:
            processResult = processGetRelaventTaskList(request);
            break;
        case Service::getSpecificTask:
            processResult = processGetSpecificTask(request);
            break;
        case Service::submitTask:
            processResult = processSubmitTask(request);
            break;
        case Service::registerCourse:
            processResult = studentApplicationProcess(
                            request, Config::ApplicationType::studentRegisterCourse);
            break;
        case Service::getTeachingCourse:
            processResult = processGetTeachingCourse(request);
            break;
        case Service::getAssignedTaskList:
            processResult = processGetAssignedTaskList(request);
            break;
        case Service::selectCourseToTeach:
            processResult = teacherApplicationProcess(request, Config::ApplicationType::teacherRegisterCourse);
            break;
        case Service::getStudentsForTask:
            processResult = processGetStudentsForTask(request);
            break;
        case Service::getSpecificAnswer:
            processResult = processGetSpecificAnswer(request);
            break;
        case Service::gradeStudent:
            processResult = processGradeStudent(request);
            break;
        case Service::addTask:
            processResult = processAddTask(request);
            break;
        case Service::getStudents:
            processResult = processGetStudents(request);
            break;
        case Service::getTeachers:
            processResult = processGetTeachers(request);
            break;
        case Service::approveApplication:
            processResult = processApproveApplication(request);
            break;
        case Service::declineApplication:
            processResult = processDeclineApplication(request);
            break;
        case Service::getRelaventAppList:
            processResult = processGetRelaventAppList(request);
            break;
        case Service::unRegisterCourse: {
            switch (request.getRole()) {
                case DBRequest::Role::student:
                    processResult = studentApplicationProcess(
                                    request, Config::ApplicationType::studentUnregisterCourse);
                    break;
                case DBRequest::Role::teacher:
                    processResult = teacherApplicationProcess(
                                    request, Config::ApplicationType::teacherUnregisterCourse);
                    break;
                default:
                    break;
            }
            break;
        }
        case Service::getTeacherAppList:
            processResult = processGetTeacherAppList(request);
            break;
        case Service::getStudentAppList:
            processResult = processGetStudentAppList(request);
            break;
        // TODO: add more
        default:
            break;
    }

    //-test
    ServiceManager::getInstance().getHttpServer()->respondToRequest(response, processResult);
    //-test
}

