#ifndef TEACHERCONTROLLER_H
#define TEACHERCONTROLLER_H

#include "coursemodel.h"
#include "httpclient.h"

#include <QObject>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

class TeacherController : public QObject {
    Q_OBJECT
private:
    /// @brief teacher's username
    QString username;
    Q_PROPERTY(QString username MEMBER username)
    /// @brief teacher's password
    QString password;
    Q_PROPERTY(QString password MEMBER password)
    /// @brief teacher's id
    QString identity;
    Q_PROPERTY(QString identity MEMBER identity)
    /// @brief teacher's real name
    QString realname;
    Q_PROPERTY(QString realname MEMBER realname)

    /// @brief type of the request
    enum class RequestType {
        getTeachingCourse,
        getAssignedTaskList,
        getAllCourse,
        selectCourseToTeach,
        getStudentsForTask,
        getSpecificAnswer,
        gradeStudent,
        addTask,
        getRelaventAppList,
        unregCourse,
    };
    /// @brief sent request which are not yet responsed
    QMap<quint64, QPair<HttpClient::Token, RequestType>> requestPool;

    /// @brief teaching course list model, needs to be updated manually
    QStringList teachingCourseList;
    /// @brief teaching course ids' update simutaneously with the upper one
    QStringList teachingCourseIdList;

    /// @brief assigned task list model, need to be updated manually
    QStringList assignedTaskList;
    /// @brief stores the id of the assigned tasks
    QStringList assignedTaskIdList;
    /// @brief current selected task id
    QString currentTaskId;
    Q_PROPERTY(QString currentTaskId MEMBER currentTaskId NOTIFY currentTaskIdChanged)
    QString currentTaskQuestion;
    Q_PROPERTY(QString currentTaskQuestion MEMBER currentTaskQuestion NOTIFY currentTaskQuestionChanged)

    /// @brief student's list relavent to a specific task, will be registered
    /// as relaventStudentsModel
    QStringList relaventStudnetList;
    /// @brief id list of the upper list
    QStringList relaventStudnetIdList;
    /// @brief current selected studnet's id and info (changed simutanesously)
    QString currentStudentId;
    QString currentStudentInfo;
    Q_PROPERTY(QString currentStudentInfo MEMBER currentStudentInfo NOTIFY currentStudentInfoChanged)
    QString currentTaskAnswer;
    Q_PROPERTY(QString currentTaskAnswer MEMBER currentTaskAnswer NOTIFY currentTaskAnswerChanged)

    /// @brief list to store sent applications,
    /// @details this model will be registered as "relaventAppModel"
    QStringList relaventApplicationList;
    /// @brief updates simutaneously with the upper one
    QStringList relaventApplicationIdList;
    /// @brief stores courses pending to be approved
    /// updates simutaneously with the upper one
    QSet<QString> pendingCourseList;

    /// @details model will be registered as "allCourseModel"
    CourseModel allCourses;
    CourseFilterModel filteredCourses;

    /// @brief get selected course from the server and fill in the model
    void getTeachingCourses();

    /// @brief get tasks relavent to the course this student selected and fill
    /// in the model
    void getAssignedTaskList();

    /// @brief get sent applications
    void getRelaventApplicationList();

    /// @brief courses to register
    QSet<QString> toRegisterPool;

    // TODO: add this to student
    /// @brief set basic info for a request
    DBRequest constructBasicRequest();
public:
    explicit TeacherController(QObject *parent = nullptr);

    /// @brief register types
    static void registerQmlTypes();

    /// @brief this model has to be manually updated
    void refreshTeachingCourseModel();
    /// @brief this model has to be manually updated
    void refreshAssignedTaskModel();
    /// @brief this model has to be manually updated
    void refreshRelaventStudentListModel();
    /// @brief this model has to be manually updated
    void refreshRelaventApplicationModel();
    /// @brief this only need to be registerd once
    void exportAllCourseModel();

    /// @brief initialize datas
    Q_INVOKABLE void initialize();

    Q_INVOKABLE bool getIsCourseSelected(QString courseId);
    Q_INVOKABLE bool getIsCoursePending(QString courseId);
signals:

    /// @brief display error string
    void displayError(QString message);
    /// @brief display message string
    void displayMessage(QString message);

    // updating signal
    void currentTaskIdChanged(QString newId);
    void currentTaskQuestionChanged(QString newQuestion);
    void currentStudentInfoChanged(QString newId);
    void currentTaskAnswerChanged(QString newAnswer);
public slots:
    /// @brief get all courses's info
    void updateAllCourseInfo();
    /// @brief select course
    void selectCourseToTeach();

    /// @brief respond to server answers
    void serverResponseHandler(quint64 token);

    /// @brief when checkbox checked or unchecked
    void prepareCourses(QString id, bool isRegistered);

    /// @brief get student list according to task list
    void getStudentsForTask(int index);
    void getStudentsForTask(QString taskId);
    /// @brief get answer by task id and student id
    void getSpecificAnswer(int index);
    void getSpecificAnswer(QString studentId);

    /// @brief give this student grade
    void gradeStudent(int grade);

    /// @brief add task
    void addTask(int courseIndex, QString question);

    void unregisterCourse(int index);
};

#endif // TEACHERCONTROLLER_H
