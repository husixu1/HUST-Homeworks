/// @file database.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/20R
#include "database.h"

DataBase::DataBase() {
    QDir appPath(QCoreApplication::applicationDirPath());

    if (!appPath.cd(resDataBasePath)) {
        //> try to make that path
        qInfo() << "database dir does not exist, try to create...";
        if (appPath.mkpath(resDataBasePath)) {
            qInfo() << "path " << appPath.path() + "/" + resDataBasePath << " created";
            this->dataBasePath = resDataBasePath;
        } else {
            qWarning() << "failed to make path " << appPath.path() + "/" + resDataBasePath << ", Data connot stored";
            this->dataBasePath = resDataBasePath;
        }
    } else {
        qInfo() << "using path " << appPath.path() << " as database root path";
        this->dataBasePath = resDataBasePath;
    }

    //> set variables
    encryptKey = nullptr;
}

DataBase::~DataBase() {
    delete encryptKey;
}

void DataBase::setEncryptKey(const uint8_t *value) {
    if (encryptKey)
        delete encryptKey;

    encryptKey = new uint8_t [16];
    memcpy(encryptKey, value, 16);
}

DataBase &DataBase::getInstance() {
    static DataBase instance;
    return instance;
}

bool DataBase::addUser(QSharedPointer<DataBase::UserInfo> user) {
    if (emailMap.contains(user->email))
        return false;
    else {
        dataBaseLock.lock();
        emailMap.insert(user->email, user);
        dataBaseLock.unlock();
        emit dataBaseChanged();
        return true;
    }
}

bool DataBase::delUser(QString email) {
    dataBaseLock.lock();
    if (emailMap.contains(email)) {
        //> remove from address map if user is online
        if (emailMap[email]->isOnline) {
            QString addressPort = emailMap[email]->address.toString() + QString::number(emailMap[email]->connectedPort);
            if (onlineMap.contains(addressPort))
                onlineMap.remove(addressPort);
        }
        //> remove from database
        emailMap.remove(email);
        dataBaseLock.unlock();
        emit dataBaseChanged();
        return true;
    }
    dataBaseLock.unlock();
    return false;
}

const DataBase::UserInfo *DataBase::getUser(QString email) const {
    if (emailMap.contains(email))
        return emailMap[email].data();
    else
        return nullptr;

}

const DataBase::UserInfo *DataBase::getUser(QHostAddress address, quint64 port) const {
    QString addressPort = address.toString() + QString::number(port);

    if (onlineMap.contains(addressPort))
        return onlineMap[addressPort].data();
    else
        return nullptr;
}

const QList<const DataBase::UserInfo *> DataBase::getUserList() {
    QList<const UserInfo *> res;

    dataBaseLock.lock();
    for (auto i : emailMap.values())
        res.push_back(i.data());
    dataBaseLock.unlock();

    return res;
}

const QList<const DataBase::UserInfo *> DataBase::getOnlineUserList() {
    QList<const UserInfo *> res;

    //> return the online users
    dataBaseLock.lock();
    for (auto i : onlineMap) {
        res.push_back(i.data());
    }
    dataBaseLock.unlock();
    return res;
}


bool DataBase::modifyUser(QString email, QSharedPointer<DataBase::UserInfo> newUserInfo) {
    // TODO: if not found
    QSharedPointer<UserInfo> oldUser = emailMap[email];
    dataBaseLock.lock();

    for (QString friendMail : oldUser->friends) {
        //> replace all friend's email with the new email
        emailMap[friendMail]->friends.remove(email);
        emailMap[friendMail]->friends.insert(newUserInfo->email);
    }
    //> move some old user's info to the new one (guranteed unchange)
    newUserInfo->friends = oldUser->friends;
    newUserInfo->cachedFile = oldUser->cachedFile;
    newUserInfo->cachedMessage = oldUser->cachedMessage;

    //> update online map
    if (oldUser->isOnline) {
        onlineMap.remove(oldUser->address.toString() + QString::number(oldUser->connectedPort));
        onlineMap.insert(oldUser->address.toString() + QString::number(oldUser->connectedPort), newUserInfo);
    }
    //> update database
    emailMap.remove(email);
    emailMap.insert(newUserInfo->email, newUserInfo);

    dataBaseLock.unlock();
    emit dataBaseChanged();
    return true;
}

bool DataBase::updateOnlineStatus(
        QString email, bool isOnline, QHostAddress address,
        quint16 connectPort, quint16 listenPort) {
    if (emailMap.contains(email)) {
        dataBaseLock.lock();
        // test user if is already online
        if (emailMap[email]->isOnline) {
            // if is already online
            if (isOnline) {
                //> remove from existing map
                onlineMap.remove(emailMap[email]->address.toString()
                        + QString::number(emailMap[email]->connectedPort) );
                //> add to online map
                emailMap[email]->isOnline = isOnline;
                emailMap[email]->address = address;
                emailMap[email]->connectedPort = connectPort;
                emailMap[email]->listenPort = listenPort;
                onlineMap.insert(address.toString() + QString::number(connectPort), emailMap[email]);
            } else {
                emailMap[email]->isOnline = isOnline;
                // if is offline, remove from online map
                onlineMap.remove(emailMap[email]->address.toString()
                        + QString::number(emailMap[email]->connectedPort) );
            }
        } else {
            // if is not already online
            if (isOnline) {
                //> add to online map
                emailMap[email]->isOnline = isOnline;
                emailMap[email]->address = address;
                emailMap[email]->connectedPort = connectPort;
                emailMap[email]->listenPort = listenPort;
                onlineMap.insert(address.toString() + QString::number(connectPort), emailMap[email]);
            }
            //> else do nothing
        }
        dataBaseLock.unlock();
        emit dataBaseChanged();
        return true;
    }
    return false;
}

bool DataBase::cacheMessage(QString fromWho, QString toWho, QString message) {

    if (emailMap.contains(toWho)) {
        dataBaseLock.lock();
        emailMap[toWho]->cachedMessage.push_back({fromWho, message});
        dataBaseLock.unlock();
        return true;
    }
    return false;
}

bool DataBase::cacheFile(QString fromWho, QString toWho, QString filename) {
    if (emailMap.contains(toWho)) {
        dataBaseLock.lock();
        emailMap[toWho]->cachedMessage.push_back({fromWho, filename});
        dataBaseLock.unlock();
        emit dataBaseChanged();
        return true;
    }
    return false;
}

bool DataBase::clearCachedFileAndEmail(QString userEmail) {
    dataBaseLock.lock();
    //> return false if user not found
    if (!emailMap.contains(userEmail))
        return false;
    //> clear cache
    emailMap[userEmail]->cachedMessage.clear();
    emailMap[userEmail]->cachedFile.clear();
    dataBaseLock.unlock();
    return true;
}

bool DataBase::serialize() {
    QDir rootDir(QCoreApplication::applicationDirPath());

    if (!rootDir.cd(dataBasePath)) {
        qWarning() << "failed to cd into " << rootDir.path() + "/" + dataBasePath
                << endl << "cannot save file";
        return false;
    }

    dataBaseLock.lock();
    for (auto info : emailMap) {
        // cd in to the user's space
        if (!rootDir.cd(info->email)) {
            if (!rootDir.mkdir(info->email)) {
                qWarning() << "failed to make dir " << info->email << " skipping";
                return false;
            }
            rootDir.cd(info->email);
        }
        QJsonObject userInfo;
        QJsonArray friendsEmail;

        //> construct userinfojson
        userInfo.insert("email", info->email);
        userInfo.insert("username", info->username);
        userInfo.insert("passwd", info->passwd);
        userInfo.insert("avatar", info->avatar);
        userInfo.insert("question", info->safeQuestion);
        userInfo.insert("answer", info->safeQuestionAnswer);
        for (auto friendEmail : info->friends)
            friendsEmail.append(friendsEmail);
        userInfo.insert("friends", friendsEmail);


        //> serialize cached message
        quint64 counter = 0;
        QJsonObject cachedMessage;
        for (auto message : info->cachedMessage) {
            cachedMessage.insert(message.first + "_" + QString::number(counter), message.second);
            counter++;
        }

        //> serialize cached file
        counter = 0;
        QJsonObject cachedFile;
        for (auto file : info->cachedFile) {
            cachedFile.insert(file.first + "_" + QString::number(counter), file.second);
            counter++;
        }
        userInfo.insert("messages", cachedMessage);
        userInfo.insert("files", cachedFile);

        //> encrypt and write to file
        QByteArray encrytedData = AesEncrypt(QJsonDocument(userInfo).toJson(), encryptKey);

        QFile userInfoFile(rootDir.path() + "/" + resUserInfoFileName);
        userInfoFile.open(QIODevice::WriteOnly);
        userInfoFile.write(encrytedData);
        userInfoFile.close();

        //> get to the last dir
        rootDir.cdUp();
    }
    dataBaseLock.unlock();
    return true;
}

bool DataBase::deSerialize() {
    QDir rootDir(QCoreApplication::applicationDirPath());

    dataBaseLock.lock();

    // ignore unexist path
    if (!rootDir.cd(dataBasePath)) {
        qDebug() << rootDir.path() << " does not exist, returning";
        dataBaseLock.unlock();
        return false;
    }

    // iterate throuth all directory
    for (auto entry : rootDir.entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot)) {
        // check if there's a valid dir
        if (!rootDir.cd(entry)) {
            qInfo() << "cannot cd into directory " << entry << " skipping";
            continue;
        }

        // check if there's a valid userinfo file
        if (!rootDir.entryList(QStringList(), QDir::Files).contains(resUserInfoFileName)) {
            qInfo() << "not valid dir " << rootDir.path() << " skipping. " << endl
                    << "We strongly do not recommend adding anything else to DataBase path";
            rootDir.cdUp();
            continue;
        }

        qDebug() << "valid path " << rootDir.path();
        //> open the file and read raw datas
        QFile userInfoFile(rootDir.path() + "/" + resUserInfoFileName);
        userInfoFile.open(QIODevice::ReadOnly);
        QByteArray encryptedData = userInfoFile.readAll();
        userInfoFile.close();

        //> decrypt
        QJsonObject userInfo = QJsonDocument::fromJson(AesDecrypt(encryptedData, encryptKey)).object();

        //> try to read the user info if user is not in the map
        QSharedPointer<UserInfo> info(new UserInfo);
        info->email = userInfo["email"].toString();
        if (emailMap.contains(info->email)) {
            rootDir.cdUp();
            continue;
        }

        //> read info
        info->username = userInfo["username"].toString();
        info->passwd = userInfo["passwd"].toString();
        info->avatar = userInfo["avatar"].toString();
        info->safeQuestion = userInfo["question"].toString();
        info->safeQuestionAnswer = userInfo["answer"].toString();
        for (auto friendEmail : userInfo["friends"].toArray())
            info->friends.insert(friendEmail.toString());
        for (QString userEmail : userInfo["messages"].toObject().keys()) {
            info->cachedMessage.append( {
                userEmail.left(userEmail.lastIndexOf('_')),
                userInfo["messages"].toObject()[userEmail].toString()
            });
        }
        for (QString userEmail : userInfo["files"].toObject().keys()) {
            info->cachedFile.append({
                userEmail.left(userEmail.lastIndexOf('_')),
                userInfo["files"].toObject()[userEmail].toString()
            });
        }

        //> insert to map
        emailMap.insert(info->email, info);

        rootDir.cdUp();
        continue;
    }
    dataBaseLock.unlock();
    emit dataBaseChanged();
    return true;
}

DataBase::UserInfo::UserInfo() {
    isOnline = false;
    address = QHostAddress::Null;
    listenPort = 0;
    connectedPort = 0;
}


QByteArray DataBase::AesEncrypt(QByteArray data, const uint8_t *key) {
    AES aes;
    aes.SetParameters(128);
    aes.StartEncryption(key);

    //> 16 bytes padding
    char appendSize = 16 - (data.size() % 16);
    data.append(static_cast<int>(appendSize), appendSize);

    //> start encrption
    uint8_t *output = new uint8_t [data.size() + 1];
    aes.Encrypt(reinterpret_cast<const uint8_t *>(data.data()), output, data.size() / 16);
    QByteArray res(reinterpret_cast<char *>(output), data.size());
    delete output;
    return res;
}

QByteArray DataBase::AesDecrypt(QByteArray data, const uint8_t *key) {
    AES aes;
    aes.SetParameters(128);
    aes.StartDecryption(key);

    //> decrypt
    uint8_t *output = new uint8_t [data.size() + 1];
    aes.Decrypt(reinterpret_cast<const uint8_t *>(data.data()), output, data.size() / 16);
    QByteArray res(reinterpret_cast<char *>(output), data.size());

    //> truncate the uneeded bytes
    res.truncate(res.size() - static_cast<int>(res.at(res.size() - 1)));
    delete output;
    return res;
}
