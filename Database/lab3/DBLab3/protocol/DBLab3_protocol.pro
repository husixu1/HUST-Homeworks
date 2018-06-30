#-------------------------------------------------
#
# Project created by QtCreator 2018-05-19T18:35:17
#
#-------------------------------------------------

include(../DBLab3_common.pri)
DESTDIR = $$BUILDDIR_static_libs

QT       -= gui
QT       += quick

TARGET = DBLab3_protocol
TEMPLATE = lib
CONFIG += staticlib
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    dbrequest.cpp \
    dbresponse.cpp \
    protocolconstants.cpp

HEADERS += \
    dbrequest.h \
    dbresponse.h \
    protocolconstants.h

unix {
    target.path = /usr/local/lib
    INSTALLS += target
}
