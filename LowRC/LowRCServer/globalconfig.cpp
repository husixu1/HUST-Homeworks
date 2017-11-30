/// @file globalconfig.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "globalconfig.h"

static const QMap<GlobalConfig::ConfigKey, QString> configKeyMap = {
    {GlobalConfig::keyEncryptKeyHash, "encryptKeyHash"},
    {GlobalConfig::keyDatabasePath, "databasePath"},
    {GlobalConfig::keyUserInfoFileName, "userInfoFileName"},
};



GlobalConfig &GlobalConfig::getInstance() {
    static GlobalConfig instance;
    return instance;
}

bool GlobalConfig::loadConfig() {
    //> try open config file
    QFile configFile(QApplication::applicationDirPath() + "/" + resConfigFilePath);
    if (!configFile.exists())
        return false;
    if (!configFile.open(QIODevice::ReadOnly))
        return false;

    //> read config from file
    QByteArray configData = configFile.readAll();
    configFile.close();

    //> resolve data
    QJsonObject config = QJsonDocument::fromJson(configData).object();
    currentConfig = config;
    return true;
}

bool GlobalConfig::saveConfig() {
    //> try open config file
    QFile configFile(QApplication::applicationDirPath() + "/" + resConfigFilePath);
    if (!configFile.open(QIODevice::WriteOnly))
        return false;

    //> write config to file
    configFile.write(QJsonDocument(currentConfig).toJson());
    configFile.close();
    return true;
}

QJsonValue GlobalConfig::getValue(GlobalConfig::ConfigKey key) {
    return currentConfig[configKeyMap[key]];
}

bool GlobalConfig::contains(GlobalConfig::ConfigKey key) {
    return currentConfig.contains(configKeyMap[key]);
}

void GlobalConfig::setVaule(GlobalConfig::ConfigKey key, QJsonValue value) {
    if (currentConfig.contains(configKeyMap[key]))
        currentConfig[configKeyMap[key]] = value;
    else
        currentConfig.insert(configKeyMap[key], value);
}
