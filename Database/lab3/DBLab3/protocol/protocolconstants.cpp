#include "protocolconstants.h"

namespace Config {
namespace TableName {
const QString manager("Manager");
const QString teacher("Teacher");
const QString student("Student");
const QString course("Course");
const QString task("Task");
const QString studentCourse("StudentCourse");
const QString studentTask("StudentTask");
const QString teacherCourse("TeacherCourse");
const QString teacherApplication("TeacherApplication");
const QString studentApplication("StudentApplication");
}

namespace ColumnName {
namespace Manager {
const QString id("id");
const QString name("name");
const QString username("username");
const QString password("password");
}
namespace Teacher {
const QString id("id");
const QString name("name");
const QString username("username");
const QString password("password");
}
namespace Student {
const QString id("id");
const QString name("name");
const QString username("username");
const QString password("password");
}
namespace Course {
const QString id("id");
const QString name("name");
const QString span("span");
const QString season("season");
}
namespace Task {
const QString id("id");
const QString courseId("courseId");
const QString teacherId("teacherId");
const QString question("question");
const QString startTime("start");
const QString endTime("end");
const QString type("type");
}
namespace StudentCourse {
const QString studentId("studentId");
const QString courseId("courseId");
}
namespace StudentTask {
const QString studentId("studentId");
const QString taskId("taskId");
const QString answer("answer");
const QString score("score");
const QString finishTime("finishTime");
}
namespace TeacherCourse {
const QString teacherId("teacherId");
const QString courseId("courseId");
}
namespace StudentApplication {
const QString id("id");
const QString statement("statement");
const QString studentId("studentId");
const QString isApproved("approved");
const QString managerId("managerId");
}
namespace TeacherApplication {
const QString id("id");
const QString statement("statement");
const QString teacherId("teacherId");
const QString isApproved("approved");
const QString managerId("managerId");
}
}

namespace ApplicationType {
const QString studentRegisterCourse("studentRegCourse");
const QString studentUnregisterCourse("studentUnregCourse");
const QString teacherRegisterCourse("teacherRegCourse");
const QString teacherUnregisterCourse("teacherUnregCourse");
const QString teacherAddCourse("teacherAddCourse");
}

namespace ApplicationRole {
const QString teacher("teacher");
const QString student("student");
}

}
