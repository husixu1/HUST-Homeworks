#-------------------------------------------------
#
# Project created by QtCreator 2017-09-25T02:56:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LowRCClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    UI/login/logindialog.cpp \
    UI/res/res.cpp \
    UI/utils/clickablelabel.cpp \
    UI/utils/systemtray.cpp \
    UI/utils/titlebutton.cpp \
    main.cpp \
    UI/login/registerdialog.cpp \
    core/clientcore.cpp \
    UI/login/settingsdialog.cpp \
    UI/main/mainwindow.cpp \
    UI/utils/animations.cpp \
    core/database.cpp \
    UI/utils/userinfodisplay.cpp \
    UI/main/chatpage.cpp \
    UI/utils/chatbubble.cpp \
    UI/utils/fileinfodisplay.cpp \
    UI/login/passworddialog.cpp

HEADERS += \
    UI/login/logindialog.h \
    UI/res/res.h \
    UI/utils/clickablelabel.h \
    UI/utils/systemtray.h \
    UI/utils/titlebutton.h \
    UI/login/registerdialog.h \
    core/clientcore.h \
    UI/login/settingsdialog.h \
    UI/main/mainwindow.h \
    UI/utils/animations.h \
    core/database.h \
    UI/utils/userinfodisplay.h \
    UI/main/chatpage.h \
    UI/utils/chatbubble.h \
    UI/utils/fileinfodisplay.h \
    UI/login/passworddialog.h

FORMS += \
    UI/login/logindialog.ui \
    UI/utils/titlebutton.ui \
    UI/login/registerdialog.ui \
    UI/login/settingsdialog.ui \
    UI/main/mainwindow.ui \
    UI/utils/userinfodisplay.ui \
    UI/main/chatpage.ui \
    UI/utils/chatbubble.ui \
    UI/utils/fileinfodisplay.ui \
    UI/login/passworddialog.ui

RESOURCES += \
    res/res.qrc

INCLUDEPATH += \
    UI \
    UI/login \
    UI/utils \
    UI/main \
    UI/res \
    core \
    core/utils \
    core/utils/log \


DISTFILES +=

QMAKE_CXXFLAGS += -std=c++14 -Wno-unused-variable
# -static -static-libgcc -static-libstdc++

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2-Release/release/ -lLowRCProtocol
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/debug/ -lLowRCProtocol
#else:unix: LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/ -lLowRCProtocol
#
#INCLUDEPATH += $$PWD/../LowRCProtocol
#DEPENDPATH += $$PWD/../LowRCProtocol
#
#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Release/release/libLowRCProtocol.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/debug/libLowRCProtocol.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Release/release/LowRCProtocol.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/debug/LowRCProtocol.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2-Debug/libLowRCProtocol.a


unix|win32: LIBS += -L$$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release/ -lLowRCProtocol

INCLUDEPATH += $$PWD/../LowRCProtocol
DEPENDPATH += $$PWD/../LowRCProtocol

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release/LowRCProtocol.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../build-LowRCProtocol-MinGW_Msys2_static-Release/release/libLowRCProtocol.a
