#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "httpclient.h"
#include "coursemodel.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QPair>

/// @brief student page controller class
// TODO: this should be a singleton class
class StudentController : public QObject {
    Q_OBJECT
private:
    /// @brief student's username
    QString username;
    Q_PROPERTY(QString username MEMBER username)
    /// @brief student's password
    QString password;
    Q_PROPERTY(QString password MEMBER password)
    /// @brief student's id
    QString identity;
    Q_PROPERTY(QString identity MEMBER identity)
    /// @brief student's real name
    QString realname;
    Q_PROPERTY(QString realname MEMBER realname)

    QString currentTaskQuestion;
    Q_PROPERTY(QString currentTaskQuestion MEMBER currentTaskQuestion NOTIFY currentTaskQuestionChanged)
    QString currentTaskAnswer;
    Q_PROPERTY(QString currentTaskAnswer MEMBER currentTaskAnswer NOTIFY currentTaskAnswerChanged)
    QString currentTaskId;
    Q_PROPERTY(QString currentTaskId MEMBER currentTaskId NOTIFY currentTaskIdChanged)
    bool isCurrentTaskSubmitted;
    Q_PROPERTY(bool isCurrentTaskSubmitted MEMBER isCurrentTaskSubmitted NOTIFY isCurrentTaskSubmittedChanged)
    // TODO: set teacher, starttime, endtime and previous answer(if exist, adding this will change sql statement)

    /// @brief type of the request
    enum class RequestType {
        getSelectedCourse,
        getAllCourse,
        getRelaventTask,
        getSpecificTask,
        submitTask,
        selectCourse,
        getRelaventAppList,
        unregCourse,
        getSpecificAnswer,
    };
    /// @brief sent request which are not yet responsed
    QMap<quint64, QPair<HttpClient::Token, RequestType>> requestPool;

    /// @brief list to store all selected course
    /// @details model will be regitered as "selectedCourseModel"
    QStringList selectedCourseList;
    /// @brief this list will be updated simutaneously with the upper one
    QStringList selectedCourseIdList;

    /// @brief list to store all relavent task
    /// @details model will be registered as "relaventTaskModel"
    QStringList relaventTaskList;
    /// @brief map index to taskid, update simutaneously with the upper TaskList
    QStringList relaventTaskIdList;

    /// @brief list to store sent applications,
    /// @details this model will be registered as "relaventAppModel"
    QStringList relaventApplicationList;
    /// @brief updates simutaneously with the upper one
    QStringList relaventApplicationIdList;
    /// @brief stores courses pending to be approved
    /// updates simutaneously with the upper one
    QSet<QString> pendingCourseList;

    /// @brief store all courses
    /// @details model will be registered as "allCourseModel"
    CourseModel allCourses;
    CourseFilterModel filteredCourses;

    /// @brief courses to register
    QSet<QString> toRegisterPool;

    /// @brief get selected course from the server and fill in the model
    void getSelectedCourse();

    /// @brief get tasks relavent to the course this student selected and fill
    /// in the model
    void getRelaventTaskList();

    /// @brief get sent applications
    void getRelaventApplicationList();

    /// @brief get answer and grade of this task
    void getSpecificAnswer();

    DBRequest constructBasicRequest();
public:

    explicit StudentController(QObject *parent = nullptr);
    /// @brief register types
    static void registerQmlTypes();

    /// @brief this model has to be manually updated
    void refreshSelectedCourseModel();
    /// @brief this model has to be manually updated
    void refreshRelaventTaskModel();
    /// @brief this model has to be manually updated
    void refreshRelaventApplicationModel();
    /// @brief this only need to be registerd once
    void exportAllCourseModel();

    /// @brief initialize things, such as register models
    Q_INVOKABLE void initialize();

    Q_INVOKABLE bool getIsCourseSelected(QString courseId);
    Q_INVOKABLE bool getIsCoursePending(QString courseId);
signals:
    /// @brief display error text
    void displayError(QString message);
    /// @brief display message text
    void displayMessage(QString message);

    // notify signals
    void currentTaskQuestionChanged(QString newQuestion);
    void currentTaskAnswerChanged(QString newAnswer);
    void currentTaskIdChanged(QString newAnswer);
    void isCurrentTaskSubmittedChanged(bool newVal);

private slots:
    /// @brief handle server responses
    void serverResponseHandler(quint64 token);

public slots:
    /// @brief get all courses's info
    void updateAllCourseInfo();

    /// @brief select course
    void selectCourse();

    /// @brief get one specific task
    /// @param[in] index the task index in the display list
    void getSpecificTask(int index);

    /// @brief submit tasks
    void submitTask(QString answer);

    /// @brief when checkbox checked or unchecked
    void prepareRegister(QString id, bool isRegistered);

    /// @brief send a unregister request
    void unregisterCourse(int index);
};

#endif // STUDENTCONTROLLER_H
