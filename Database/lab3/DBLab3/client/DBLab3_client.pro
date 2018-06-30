include(../DBLab3_common.pri)
QT += quick quickcontrols2
CONFIG += c++11 c++14

DESTDIR = $$BUILDDIR_client

DEFINES += QT_DEPRECATED_WARNINGS QHTTP_HAS_CLIENT

SOURCES += \
        main.cpp \
    logincontroller.cpp \
    globalconfig.cpp \
    httpclient.cpp \
    studentcontroller.cpp \
    teachercontroller.cpp \
    coursemodel.cpp \
    managercontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    qtquickcontrols2.conf

HEADERS += \
   logincontroller.h \
    globalconfig.h \
    httpclient.h \
    studentcontroller.h \
    teachercontroller.h \
    coursemodel.h \
    managercontroller.h

# search from current director
QMAKE_LFLAGS = "-Wl,-rpath,\'\$$ORIGIN/lib\'"

INCLUDEPATH += $$PROJECT_ROOT/libs/qhttp
INCLUDEPATH += $$PROJECT_ROOT/protocol

LIBS += -L$$BUILDDIR_libs -lqhttp

# link protocol
LIBS += -L$$BUILDDIR_static_libs -lqhttp -lDBLab3_protocol

# copy libraries
QMAKE_POST_LINK = rm -rf $$BUILDDIR_client/lib
QMAKE_POST_LINK = cp -r $$BUILDDIR_libs $$BUILDDIR_client
