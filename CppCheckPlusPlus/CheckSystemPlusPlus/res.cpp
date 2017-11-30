/// @file res.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "res.h"

const QString Resources::resStandardExecDirPath = "refProg";
const QString Resources::resStandardStackExecName = "a.exe";
const QString Resources::resStandardQueueExecName = "b.exe";
const QString Resources::resStandardQueueByStackExecName = "c.exe";

const QStringList Resources::resDefaultTestCases = {
    "-S  5 -I  1  2  3  4 -O  2 -O  2",
    "-S  5 -O  0 -I  1  2  3  4 -O  5 -I  1",
    "-S  5 -I  1  2  3  4 -O  2 -I  5  6  7 -I  8",
    "-S  5 -I  1  2  3  4 -O  2 -A  4 -I  5  6 -I  7 -I  8",
    "-S  5 -I  1  2  3  4 -O  2 -C -I  5  6 -A  2",
    "-S  5 -I  1  2  3  4 -O  2 -N",
    "-S  5 -I  1  2  3  4 -G  3 -G  7",
    "-S  5 -I  1  2  3  4 -G  3 -I  5  6  7  8 -O  3 -I  9  0 -G  6 -I  1",
    "-S  3 -I  1  2  3 -O  1 -I  5  6 -G  1 -G  6",
    "-S  3 -I  1  2  3  4 -G  1 -I  5  6 -G  5 -O  6 -O  1",
};

const quint64 Resources::resMaxUserTestCaseFileSize = 1000000;

const quint64 Resources::resTestCaseTimeOutInterval = 3000;

const QColor Resources::resWrongAnswerColor = QColor(238, 194, 212);
const QColor Resources::resAcceptedColor = QColor(181, 223, 197);
const QColor Resources::resFailedToStartColor = QColor(254, 254, 111);
const QColor Resources::resTimedOutColor = QColor(157, 159, 207);


QTranslator *Global::globalCurrentTranslator = nullptr;
