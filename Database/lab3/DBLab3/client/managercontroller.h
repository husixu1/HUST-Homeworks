#ifndef MANAGERCONTROLLER_H
#define MANAGERCONTROLLER_H

#include "httpclient.h"

#include <QObject>
#include <QMap>

class ManagerController : public QObject {
    Q_OBJECT
public:
    enum class RequestType {
        getStudentsList,
        getCoursesList,
        getTeachersList,
        executeStudentApplication,
        executeTeacherApplication,
        getTeacherAppList,
        getStudentAppList,
    };
private:
    /// @brief manager's id
    QString identity;
    Q_PROPERTY(QString identity MEMBER identity)
    /// @brief manager's username
    QString username;
    Q_PROPERTY(QString username MEMBER username)
    /// @brief manager's password
    QString password;
    Q_PROPERTY(QString password MEMBER password)
    /// @brief manager's realname
    QString realname;
    Q_PROPERTY(QString realname MEMBER realname)

    /// @brief sent request which are not yet responsed
    QMap<quint64, QPair<HttpClient::Token, RequestType>> requestPool;

    /// @brief courses info list, will be registered as courseModel
    QStringList coursesInfo;
    QStringList coursesId;

    /// @brief teacher info list, will be registered as teacherModel
    QStringList teachersInfo;
    QStringList teachersId;

    /// @brief students info list, will be reigstered as studentModel
    QStringList studentsInfo;
    QStringList studentsId;

    /// @brief teachers' applications
    QStringList teacherApplicationList;
    QStringList teacherApplicationIdList;

    /// @brief student' applications
    QStringList studentApplicationList;
    QStringList studentApplicationIdList;

    QSet<QString> teacherAppPreparePool;
    QSet<QString> studentAppPreparePool;

    /// @brief re-export course model to qml
    void refreshCourseModel();
    /// @brief re-export teacher model to qml
    void refreshTeacherModel();
    /// @brief re-export student model to qml
    void refreshStudentmodel();
    /// @brief re-export teacherapplication model to qml
    void refreshTeacherAppModel();
    /// @brief re-export teacherapplication model to qml
    void refreshStudentAppModel();

    DBRequest constructBasicRequest();
public:
    explicit ManagerController(QObject *parent = nullptr);
    static void registerQmlTypes();

signals:
    void displayError(QString message);
    void displayMessage(QString message);

public slots:
    void initialize();

    void getStudentsList();
    void getTeachersList();
    void getCoursesList();

    void getTeacherApplicationList();
    void getStudentApplicationList();

    void serverResponseHandler(quint64 token);

    /// @brief prepare to accept/decline teacher applicatoin
    void prepareTeacherApp(int index, bool checked);
    /// @brief prepare to accept/decline student applicatoin
    void prepareStudentApp(int index, bool checked);

    /// @brief send prepared list to server
    void executeStudentAppList(bool accept);
    /// @brief send prepared list to server
    void executeTeacherAppList(bool accept);
};

#endif // MANAGERCONTROLLER_H
