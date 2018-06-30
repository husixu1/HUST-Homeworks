#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

// convert keyword/identifier to string, used by things like invokeMethod
#define STRINGIFY(a) #a

#include <QString>

/// @brief global configurations, mostly static variables
namespace Config {
/// @brief name of the database used
extern const QString dataBaseName;
/// @brief database username
extern const QString databaseUserName;
/// @brief database password
extern const QString databasePassword;

/// @brief the max queued request count for an database operating thread
extern const quint32 dataBaseMaxQueuedRequest;

/// @brief the colorful log message pattern
extern const QString messagePattern;
}

#endif // GLOBALCONFIG_H

//TODO: read config from file
