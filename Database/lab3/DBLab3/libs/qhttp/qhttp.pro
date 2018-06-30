include(../../DBLab3_common.pri)
DESTDIR = $$BUILDDIR_libs

QT       += core network
QT       -= gui

TARGET    = qhttp
TEMPLATE  = lib
#CONFIG += staticlib

DEFINES       *= QHTTP_MEMORY_LOG=0 QHTTP_HAS_CLIENT
win32:DEFINES *= QHTTP_EXPORT

# Joyent http_parser
SOURCES  += http_parser.c
HEADERS  += http_parser.h

SOURCES  += \
    qhttpabstracts.cpp \
    qhttpserverconnection.cpp \
    qhttpserverrequest.cpp \
    qhttpserverresponse.cpp \
    qhttpserver.cpp

HEADERS  += \
    qhttpfwd.hpp \
    qhttpabstracts.hpp \
    qhttpserverconnection.hpp \
    qhttpserverrequest.hpp \
    qhttpserverresponse.hpp \
    qhttpserver.hpp

contains(DEFINES, QHTTP_HAS_CLIENT) {
    SOURCES += \
        qhttpclientrequest.cpp \
        qhttpclientresponse.cpp \
        qhttpclient.cpp

    HEADERS += \
        qhttpclient.hpp \
        qhttpclientresponse.hpp \
        qhttpclientrequest.hpp
}
