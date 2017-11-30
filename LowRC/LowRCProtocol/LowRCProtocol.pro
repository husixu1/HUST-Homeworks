#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T17:18:01
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = LowRCProtocol
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_DEBUG_OUTPUT

QMAKE_CXXFLAGS += -std=c++14 -O3 -Os

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baseconnector.cpp \
    operationdescriptor.cpp \
    basefiletransceiver.cpp \
    protocolconstant.cpp

HEADERS += \
    baseconnector.h \
    operationdescriptor.h \
    basefiletransceiver.h \
    protocolconstant.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
