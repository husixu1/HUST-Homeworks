#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include "dbrequest.h"
#include "httpclient.h"

#include <QObject>
#include <QString>

class LoginController : public QObject {
    Q_OBJECT
public:
    /// @brief role used to map to DBRequest::Role
    enum class Role {
        Manager,
        Teacher,
        Student
    };
    Q_ENUM(Role)

    /// @brief constructor
    explicit LoginController(QObject *parent = nullptr);

    /// @brief make available to qml files
    static void registerQmlTypes();

    /// @brief the token used to retrieve the response
    HttpClient::Token loginToken;

private:
    /// @brief record the last selected role
    Role lastSelectedRole;
    /// @brief recored the last login username
    QString lastUsername;
    /// @brief recored the last login password
    QString lastEncryptedPass;

public slots:

    /// @brief server responsed, connected to HttpClient::serverResponsed
    /// @param[in] token the token to get data from HttpClient
    void onServerResponse(quint64 token);

    // TODO: maybe change role from string to DBProtocol::Role instead
    /// @brief try login with role of manager
    /// @param[in] username login username
    /// @param[in] password login password
    /// @param[in] role login role, can be "manager", "teacher" or "student" i
    /// (This is ugly, should use protocol instead, but enum class from
    /// other class cannot be used in qml elegantly)
    void tryLogin(QString username, QString password, Role role);

signals:
    /// @brief display error message
    /// @param[out] the message to display
    void displayError(QString message);

    /// @brief close current window and login to manager/student/teacher window
    /// @param[out] role the login role
    /// @param[out] user the login username
    /// @param[out] pass the hashed password
    /// @param[out] id the student it
    /// @param[out] realName the student Realname
    void login(Role role, QString user, QString pass, QString identity, QString realName);
};

#endif // LOGINCONTROLLER_H
