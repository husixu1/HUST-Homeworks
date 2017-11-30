/// @file globalconfig.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include "res.h"
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QApplication>

/// @brief a class used to load, save, change and do other operation on configurations
/// configuration is in json format
class GlobalConfig {
public:
    /// @brief possible config keys
    enum ConfigKey {
        keyEncryptKeyHash,
        keyDatabasePath,
        keyUserInfoFileName,
    };

private:
    /// @brief the very global config
    QJsonObject config;

    GlobalConfig() = default;

    QJsonObject currentConfig;
public:

    /// @brief get the singleton instance of this class
    static GlobalConfig &getInstance();

    /// @brief load configuratoin
    /// load configuration from pre-configured configuration path (defined in res.h)
    /// @return if load is successful
    bool loadConfig();

    /// @brief save current configuration
    /// save configuratoin to pre-configured configuration path (defined in res.h).
    /// this will overwrite the old config.
    /// @return if the saving successful
    bool saveConfig();

    /// @brief get a value from current config file
    QJsonValue getValue(ConfigKey key);

    /// @brief judge if key exists in global config
    bool contains(ConfigKey key);

    /// @brief set value
    void setVaule(ConfigKey key, QJsonValue value);
};

#endif // GLOBALCONFIG_H
