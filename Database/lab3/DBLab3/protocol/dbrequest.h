#ifndef DBLAB3_PROTOCOL_H
#define DBLAB3_PROTOCOL_H

#include <QJsonObject>
#include <QString>
#include <QMap>
#include <QVariant>

/// @brief the request used by both the client and the server
/// @details The client create and fill a request, convert it to Json using toJson(),
/// and send it to server; The server get request from json using fromJson(),
/// then process the request and send a respasonse to client
class DBRequest {
public:
    /// @brief possible roles of the request maker
    enum class Role {
        invalid,
        student,
        teacher,
        manager,
    };

    // TODO: switch this struct to union and fill the union with additional key-val
    /// @brief all services can be required by the clients
    /// @details what services can be provided to what roles are defined
    /// by the server program
    /// @note this class can be expanded, if more functions needs to be
    /// provided by the server
    /// @todo The services can be defined by the server program, and invoke
    /// those programs to process different requests, to add expansibility.
    /// (service types can be added dynamically on need, just a new external
    /// program is needed if the api/abi is fixed). We define services
    /// here in protocol just for convenience. It's just a lab so there's
    /// no need to do that much, it's too complex. (I'll consider it if
    /// it's a serious lab, but it's not).
    enum class RequestService {
        invalid,

        // common services
        login,
        getCourse,

        // student services
        getRegisteredCourse,
        getRelaventTaskList,
        getSpecificTask,
        submitTask,
        registerCourse,
        getRelaventAppList,
        unRegisterCourse,

        // teacher services
        getTeachingCourse,
        getAssignedTaskList,
        selectCourseToTeach,
        getStudentsForTask,
        getSpecificAnswer,
        gradeStudent,
        addTask,

        // manager services
        getStudents,
        getTeachers,
        approveApplication,
        declineApplication,
        getTeacherAppList,
        getStudentAppList,
    };

    /// @brief keys used by additional request data
    /// @details the requirement for requestData is defined by server program
    /// @see requestData
    enum class RequestDataKey {
        id,         ///< @brief identity of nearly anything
        taskId,     ///< @brief when id is student id, this stores task id
        answer,     ///< @brief task answer
        courseIds,  ///< @brief a list of course id
        courseId,   ///< @brief one course's id (when 'id' is used by teacher)
        grade,      ///< @brief give grade to student-task pair
        question,   ///< @brief question of the task
        appRole,    ///< @brief who proposed this application ?
        appIds,     ///< @brief a list of application id
    };

private:

    /// @brief the role of the request maker
    Role role;
    /// @brief username of the request
    QString userName;
    /// @brief password of the request
    QString password;
    /// @brief type of the request
    RequestService requestService;

    /// @brief the sql statement to execute
    /// sanity check will be performed by the sql server
    QJsonObject requestData;

public:
    /// @brief register to package: DBLab3.DBRequest, module DBRequest
    static void registerMetaTypes();

    DBRequest();

    /// @brief convert the request to json object, usually used by client.
    QJsonObject toJson() const;

    /// @brief get an HttpRequest Object from json, usually used by server.
    /// @param[in] object the object to convert
    static DBRequest fromJson(const QJsonObject &object);

    /// @brief add a key-val pair to requestData
    /// @param[in] key the key-val pair key
    /// @param[in] value the key-val pair value
    /// @see requestData
    void addToRequestData(RequestDataKey key, QJsonValue value);

    /// @brief get value from requestData
    /// @return the value if key is found, QJsonValue::Undefined when not found
    /// @throw logic_error if key-val pair is not found
    /// @see requestData
    QJsonValue getValFromRequestData(RequestDataKey key) const;

    // getters and setters
    QString getUserName() const;
    void setUserName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    Role getRole() const;
    void setRole(const Role &value);

    RequestService getRequestService() const;
    void setRequestService(const RequestService &value);
};

Q_DECLARE_METATYPE(DBRequest)
#endif // DBLAB3_PROTOCOL_H
