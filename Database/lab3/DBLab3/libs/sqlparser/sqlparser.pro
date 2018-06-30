#-------------------------------------------------
#
# Project created by QtCreator 2018-05-22T16:08:14
#
#-------------------------------------------------

include(../../DBLab3_common.pri)
DESTDIR = $$BUILDDIR_libs

QT       -= core gui

CONFIG += c++17
TARGET = sqlparser
TEMPLATE = lib

DEFINES += SQLPARSER_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    parser/bison_parser.cpp \
    parser/flex_lexer.cpp \
    SQLParser.cpp \
    SQLParserResult.cpp \
    util/sqlhelper.cpp \
    sql/PrepareStatement.cpp \
    sql/SQLStatement.cpp \
    sql/Expr.cpp \
    sql/statements.cpp

HEADERS += \
    SQLParser.h \
    SQLParserResult.h \
    util/sqlhelper.h \
    sql/statements.h \
    sql/ExecuteStatement.h \
    sql/PrepareStatement.h \
    sql/ImportStatement.h \
    sql/ShowStatement.h \
    sql/UpdateStatement.h \
    sql/DeleteStatement.h \
    sql/DropStatement.h \
    sql/InsertStatement.h \
    sql/SQLStatement.h \
    sql/CreateStatement.h \
    sql/SelectStatement.h \
    sql/Table.h \
    sql/Expr.h

QMAKE_CXXFLAGS += -Wno-sign-compare -Wno-unneeded-internal-declaration -Wno-register -Wno-unused-parameter

unix {
    target.path = /usr/lib
    INSTALLS += target
}
