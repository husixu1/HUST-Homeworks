#-------------------------------------------------
#
# Project created by QtCreator 2017-10-22T16:34:57
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LowRCServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += -std=c++14
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    servercore.cpp \
    dataprocessor.cpp \
    database.cpp \
    res.cpp \
    globalconfig.cpp \
    passworddialog.cpp \
    changepassworddialog.cpp

HEADERS += \
        mainwindow.h \
    servercore.h \
    dataprocessor.h \
    database.h \
    res.h \
    globalconfig.h \
    passworddialog.h \
    changepassworddialog.h

FORMS += \
        mainwindow.ui \
    passworddialog.ui \
    changepassworddialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/release/ -lLowRCProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/debug/ -lLowRCProtocol
else:unix: LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/ -lLowRCProtocol

INCLUDEPATH += $$PWD/../LowRCProtocol
DEPENDPATH += $$PWD/../LowRCProtocol

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/release/libLowRCProtocol.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/debug/libLowRCProtocol.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/release/LowRCProtocol.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/debug/LowRCProtocol.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/libLowRCProtocol.a

unix|win32: LIBS += -L$$PWD/lib/ -lAES

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/AES.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/libAES.a

unix|win32: LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release/ -lLowRCProtocol

INCLUDEPATH += $$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release
DEPENDPATH += $$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release/LowRCProtocol.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release/libLowRCProtocol.a
