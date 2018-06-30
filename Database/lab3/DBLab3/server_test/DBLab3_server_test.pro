#-------------------------------------------------
#
# Project created by QtCreator 2018-05-18T19:57:41
#
#-------------------------------------------------

include(../DBLab3_common.pri)
DESTDIR = $$BUILDDIR_server_test

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_dblab3_server_testtest

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    dbconnectortest.cpp \
    main.cpp \
    ../server/dbconnector.cpp \
    ../server/globalconfig.cpp

HEADERS += \
    dbconnectortest.h \
    ../server/dbconnector.h \
    ../server/globalconfig.h

INCLUDEPATH += ../server

# link qhttp
LIBS += -L$$BUILDDIR_libs -lqhttp -lsqlparser

# search from current director
QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"

# copy libraries
QMAKE_POST_LINK = cp -r $$BUILDDIR_libs $$BUILDDIR_server
