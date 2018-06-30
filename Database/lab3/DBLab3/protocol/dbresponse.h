#ifndef DBRESPONSE_H
#define DBRESPONSE_H

#include <QJsonObject>
#include <QList>

class DBResponse {
    Q_GADGET
public:
    enum class CommonErrorCode {
        noError,                ///< @brief no error occurred
        serverBusy,             ///< @brief the server is too busy to process request
        internalError,          ///< @brief server has internal error
        transactionFailed,      ///< @brief cannot execute transcation correctly
        partialInternalError,   ///< @brief some of the service executed, others not
    };
    Q_ENUM(CommonErrorCode)
    enum class ServiceErrorCode {
        noError,                ///< @brief no error occurred
        permissionCheckFailed,  ///< @brief request didn't pass permission check
        integrityCheckFailed,   ///< @brief request didn't pass integrity check
        noRole,                 ///< @brief role unidentified
        noUser,                 ///< @brief no such user registered
        wrongPassword,          ///< @brief wrong password when login
        noTask,                 ///< @brief task not found
        taskAlreadySubmitted,   ///< @brief duplicate task submit request
        courseRegisterFailed,   ///< @brief failed registering course
        studentTaskNotFound,    ///< @brief {student,task} group not found
        partialAppNotFound,     ///< @brief some application id in list not found
    };
    Q_ENUM(ServiceErrorCode)

    /// @brief extra response datas (in addition to table name)
    enum class ResponseDataKey {
        failedCourses,          ///< @brief a list of failed courses
        pendingCourseSet,       ///< @brief courses' id who is pending to be approved
    };
private:
    /// @brief general error
    CommonErrorCode commonError;

    /// @brief error caused in request processing
    ServiceErrorCode serviceError;

    /// @brief the returning table (usually for query request)
    /// @details format: { <column_name> : [value, ... ], ... }
    QJsonObject table;

public:
    /// @brief register meta types
    static void registerMetaTypes();

    /// @brief convert to json object
    /// @return the converted json object
    QJsonObject toJson();

    /// @brief construct DBResponse from json object
    /// @param[in] object the json object
    /// @return the constructed class
    static DBResponse formJson(const QJsonObject &object);

    /// @brief add extra data to table
    void addToResponseTable(ResponseDataKey key, QJsonValue value);
    /// @brief get extra data from table
    QJsonValue getFromResponseTable(ResponseDataKey key);

    /// @brief consturctor
    DBResponse();

    // setter and getter
    QJsonObject getTable() const;
    void setTable(const QJsonObject &value);
    CommonErrorCode getCommonError() const;
    void setCommonError(const CommonErrorCode &value);
    ServiceErrorCode getServiceError() const;
    void setServiceError(const ServiceErrorCode &value);
};

Q_DECLARE_METATYPE(DBResponse)

#endif // DBRESPONSE_H
