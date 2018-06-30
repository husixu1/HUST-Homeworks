#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include "dbrequest.h"
#include "dbresponse.h"
#include "qhttpserver.hpp"

#include <QObject>
#include <QString>
#include <QQueue>
#include <QJsonObject>
#include <QMutex>
#include <QMap>
#include <QSet>
#include <QPair>
#include <QSql>
#include <QtSql/QSqlDatabase>

/// @brief this class connects to the local database and operate on it
/// @details All requests from httpserver are queued using Qt QueuedConnection
/// and processed here. Config::dataBaseMaxQueuedRequset decide how many
/// requests a BDConnector instance can hold
class DBConnector : public QObject {
    Q_OBJECT
private:
    /// @brief the actucal database this program use
    QSqlDatabase database;

    /// @brief current count of the queued request
    quint32 requestCount;
    /// @brief the mutex for adding request to queue
    QMutex requestCountLock;

    // difference request proceesors, called by slot processRequest
    DBResponse processLoginRequest(const DBRequest &request);
    DBResponse processGetRegisteredCourse(const DBRequest &request);
    DBResponse processGetCourse(const DBRequest &request);
    DBResponse processGetRelaventTaskList(const DBRequest &request);
    DBResponse processGetSpecificTask(const DBRequest &request);
    DBResponse processSubmitTask(const DBRequest &request);
    DBResponse processGetTeachingCourse(const DBRequest &request);
    DBResponse processGetAssignedTaskList(const DBRequest &request);
    DBResponse processGetStudentsForTask(const DBRequest &request);
    DBResponse processGetSpecificAnswer(const DBRequest &request);
    DBResponse processGradeStudent(const DBRequest &request);
    DBResponse processAddTask(const DBRequest &request);
    DBResponse processGetStudents(const DBRequest &request);
    DBResponse processGetTeachers(const DBRequest &request);
    DBResponse processGetRelaventAppList(const DBRequest &request);
    DBResponse processGetTeacherAppList(const DBRequest &request);
    DBResponse processGetStudentAppList(const DBRequest &request);
    // TODO: add more

    // stores a process into database to be executed when manager approved
    // the execution of the process
    DBResponse studentApplicationProcess(const DBRequest &request, QString applicationType);
    DBResponse teacherApplicationProcess(const DBRequest &request, QString applicationType);

    // process to approve previously stored process
    DBResponse processApproveApplication(const DBRequest &request);
    DBResponse processDeclineApplication(const DBRequest &request);

    // second-stage services (approved/declied by manager)
    DBResponse processSelectCourseToTeach(const DBRequest &request);
    DBResponse processStudentSelectCourse(const DBRequest &request);
    DBResponse processStudentUnregisterCourse(const DBRequest &request);
    DBResponse processTeacherUnregisterCourse(const DBRequest &request);


    /// @brief calculate a new index with prefix
    /// @note this function IMPLIES dbResult.next
    QString calcNewInex(QString prefix, QSqlQuery &dbResult, QString indexColName);

    /// @brief construct a json and convert to string from request
    /// @brief used in applications
    QString constructApplicationJson(QString type, const DBRequest &request);
    /// @brief restore string to QJson Object, used in the second stage
    /// of application (approval/decline)
    /// @return a {type, original-request} pair
    QPair<QString, DBRequest> restoreApplicationJson(QString content);

    /// @brief get the human-readable string of a application
    QString getRepresentsOfApplication(QSqlQuery &dbResult,
            DBRequest::Role role, QSet<QString> &pendingCourseSet);
public:
    /// @brief constructor
    /// @param[in] name then name must be unique for different connection
    DBConnector(QString name);

    /// @brief try starting/restart an sql connection
    /// @param[in] userName name of the database user
    /// @param[in] password passworf of the database user
    /// @see connectionStarted() connectionFailed()
    /// @threadsafe
    void startConnectionLater(
            const QString &hostName,
            const QString &userName,
            const QString &password);

    /// @brief stop current connection
    /// @see connectionClosed()
    /// @threadsafe
    void stopConnectionLater();

    /// @brief add an request to the request queue
    /// When maximum queued connection count is reached, this function will
    /// not add the request to queue and will return false.
    /// @param[in] request the request to add
    /// @param[in] the response
    /// @return if the request is successfully added
    /// @threadsafe
    // TODO: add response to queue
    bool addToRequestQueue(
            DBRequest request,
            qhttp::server::QHttpResponse *response);

private:
    /// @brief This function will check if the request's permission.\n
    /// @details That is, checks if the request sender has permission to do
    /// the operatoin requested.
    /// @param[in] request
    /// @return if the request passed sanity check
    bool permissionCheck(const DBRequest &request);

    /// @brief Checks the request's integrity
    /// This function will check if the request has sufficient information
    /// to do the operation requested.
    /// @param[in] request
    /// @return if the request passed integrity check
    bool integrityCheck(const DBRequest &request);

signals:
    /// @brief make respond when the request is processed
    void respondToRequest(QByteArray data);

    /// @brief emitted when connection is started
    void connectionStarted();
    /// @brief emitted when connection failed to start
    void connectionFailed();
    /// @brief emitted when connection is closed
    void connectionClosed();

private slots:
    /// @brief start connection on current thread
    void startConnection(
            const QString &hostName,
            const QString &userName,
            const QString &password);
    /// @brief stop connection on current thread
    void stopConnection();

    /// @brief process one request in the queue
    /// @note connect to this slot using queued connection for safety
    void processRequest(
            DBRequest request,
            qhttp::server::QHttpResponse *response);
};

#endif // DBCONNECTOR_H
