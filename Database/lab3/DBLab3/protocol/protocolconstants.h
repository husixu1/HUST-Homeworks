#ifndef PROTOCOLCONSTANTS_H
#define PROTOCOLCONSTANTS_H

#include <QString>

namespace Config {
namespace TableName {
extern const QString manager;
extern const QString teacher;
extern const QString student;
extern const QString course;
extern const QString task;
extern const QString studentCourse;
extern const QString studentTask;
extern const QString teacherCourse;
extern const QString teacherApplication;
extern const QString studentApplication;
}

namespace ColumnName {
namespace Manager {
extern const QString id;
extern const QString name;
extern const QString username;
extern const QString password;
}
namespace Teacher {
extern const QString id;
extern const QString name;
extern const QString username;
extern const QString password;
}
namespace Student {
extern const QString id;
extern const QString name;
extern const QString username;
extern const QString password;
}
namespace Course {
extern const QString id;
extern const QString name;
extern const QString span;
extern const QString season;
}
namespace Task {
extern const QString id;
extern const QString courseId;
extern const QString teacherId;
extern const QString question;
extern const QString startTime;
extern const QString endTime;
extern const QString type;
}
namespace StudentCourse {
extern const QString studentId;
extern const QString courseId;
}
namespace StudentTask {
extern const QString studentId;
extern const QString taskId;
extern const QString answer;
extern const QString score;
extern const QString finishTime;
}
namespace TeacherCourse {
extern const QString teacherId;
extern const QString courseId;
}
namespace StudentApplication {
extern const QString id;
extern const QString statement;
extern const QString studentId;
extern const QString isApproved;
extern const QString managerId;
}
namespace TeacherApplication {
extern const QString id;
extern const QString statement;
extern const QString teacherId;
extern const QString isApproved;
extern const QString managerId;
}
}

/// @brief type of student/teacher applicatons
namespace ApplicationType {
extern const QString studentRegisterCourse;
extern const QString studentUnregisterCourse;
extern const QString teacherRegisterCourse;
extern const QString teacherUnregisterCourse;
extern const QString teacherAddCourse;
}

namespace ApplicationRole {
extern const QString teacher;
extern const QString student;
}
}

#endif // PROTOCOLCONSTANTS_H
