/// @file res.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef RES_H
#define RES_H

#include <QString>
#include <QStringList>
#include <QColor>
#include <QTranslator>

// All path are relative to applicatoin's binary path

/// @brief default settings
namespace Resources {

/// @brief the standard executable's storing path
extern const QString resStandardExecDirPath;
/// @brief the standard stack executable's file name
extern const QString resStandardStackExecName;
/// @brief the standard stack executable's file name
extern const QString resStandardQueueExecName;
/// @brief the standard stack executable's file name
extern const QString resStandardQueueByStackExecName;

/// @brief the default test cases (from the old checksystem v3.0)
extern const QStringList resDefaultTestCases;

/// @brief the max size of custom test file in bytes
extern const quint64 resMaxUserTestCaseFileSize;

/// @brief the timeout time in miliseconds
extern const quint64 resTestCaseTimeOutInterval;

/// @brief wrong answer color
extern const QColor resWrongAnswerColor;
/// @brief accepted answer color
extern const QColor resAcceptedColor;
/// @brief exec failed to start color
extern const QColor resFailedToStartColor;
/// @brief test case timed out error
extern const QColor resTimedOutColor;

}

/// @brief global variables
namespace Global {

extern QTranslator *globalCurrentTranslator;

}

#endif // RES_H
