#-------------------------------------------------
#
# Project created by QtCreator 2018-05-18T08:43:23
#
#-------------------------------------------------

# build into build/server
include(../DBLab3_common.pri)
DESTDIR = $$BUILDDIR_server

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBLab3
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dbconnector.cpp \
    globalconfig.cpp \
    httpserver.cpp \
    servicemanager.cpp

HEADERS += \
        mainwindow.h \
    dbconnector.h \
    globalconfig.h \
    httpserver.h \
    servicemanager.h \
    finally.h

FORMS += \
        mainwindow.ui

# link qhttp
LIBS += -L$$BUILDDIR_libs -lsqlparser -lqhttp

# link protocol
LIBS += -L$$BUILDDIR_static_libs -lDBLab3_protocol

# search from current director
QMAKE_LFLAGS = "-Wl,-rpath,\'\$$ORIGIN/lib\'"

INCLUDEPATH += $$PROJECT_ROOT/server
INCLUDEPATH += $$PROJECT_ROOT/libs/qhttp
INCLUDEPATH += $$PROJECT_ROOT/libs/sqlparser
INCLUDEPATH += $$PROJECT_ROOT/protocol

# enable c++11, c++14
CONFIG += c++11 c++14

# copy libraries
QMAKE_POST_LINK = rm -rf $$BUILDDIR_server/lib
QMAKE_POST_LINK = cp -r $$BUILDDIR_libs $$BUILDDIR_server
