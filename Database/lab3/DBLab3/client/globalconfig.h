#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QString>
#include <QQmlApplicationEngine>

// convert keyword/identifier to string, used by things like invokeMethod
#define STRINGIFY(a) #a
#define MIN(a, b) ((a) < (b) ? (a) : (b))

namespace Config {
extern const QString messagePattern;

/// @brief address and port of the server
extern const QString serverUrl;

/// @brief the global qml engine
extern QQmlApplicationEngine *qmlEngine;
}

#endif // GLOBALCONFIG_H
