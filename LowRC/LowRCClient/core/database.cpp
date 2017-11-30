/// @file database.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "database.h"

DataBase::DataBase() {
    QDir appPath(QCoreApplication::applicationDirPath());
    if (!appPath.cd(resCachePath)) {
        //> try to make that path
        qInfo() << "database dir does not exist, try to create...";
        if (appPath.mkpath(resCachePath)) {
            qInfo() << "path " << appPath.path() + "/" + resCachePath << " created";
            this->cachePath = resCachePath;
        } else {
            qWarning() << "failed to make path " << appPath.path() + "/" + resCachePath << ", Data connot stored";
            this->cachePath = resCachePath;
        }
    } else {
        qInfo() << "using path " << appPath.path() << " as database root path";
        this->cachePath = resCachePath;
    }
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
            QString addressPort = emailMap[email]->address.toString() + QString::number(emailMap[email]->port);
            if (addrPortMap.contains(addressPort))
                addrPortMap.remove(addressPort);
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

    if (addrPortMap.contains(addressPort))
        return addrPortMap[addressPort].data();
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

//bool DataBase::modifyUser(QString email, QSharedPointer<DataBase::UserInfo> newUserInfo) {
//    // TODO: if not found
//    QSharedPointer<UserInfo> oldUser = emailMap[email];
//    dataBaseLock.lock();
//
//    for (QString friendMail : oldUser->friends) {
//        //> replace all friend's email with the new email
//        emailMap[friendMail]->friends.remove(email);
//        emailMap[friendMail]->friends.insert(newUserInfo->email);
//    }
//    //> move some old user's info to the new one (guranteed unchange)
//    newUserInfo->friends = oldUser->friends;
//    newUserInfo->cachedFile = oldUser->cachedFile;
//    newUserInfo->cachedMessage = oldUser->cachedMessage;
//
//    //> update online map
//    if (oldUser->isOnline) {
//        addrPortMap.remove(oldUser->address.toString() + QString::number(oldUser->port));
//        addrPortMap.insert(oldUser->address.toString() + QString::number(oldUser->port), newUserInfo);
//    }
//    //> update database
//    emailMap.remove(email);
//    emailMap.insert(newUserInfo->email, newUserInfo);
//
//    dataBaseLock.unlock();
//    emit dataBaseChanged();
//    return true;
//}

bool DataBase::updateOnlineStatus(QString email, bool isOnline, QHostAddress address, quint16 port) {
    if (emailMap.contains(email)) {
        dataBaseLock.lock();
        // test user if is already online
        if (emailMap[email]->isOnline) {
            // if is already online
            if (isOnline) {
                //> remove from existing map
                addrPortMap.remove(emailMap[email]->address.toString()
                        + QString::number(emailMap[email]->port) );
                //> add to online map
                emailMap[email]->isOnline = isOnline;
                emailMap[email]->address = address;
                emailMap[email]->port = port;
                addrPortMap.insert(address.toString() + QString::number(port), emailMap[email]);
            } else {
                emailMap[email]->isOnline = isOnline;
                // if is offline, remove from online map
                addrPortMap.remove(emailMap[email]->address.toString()
                        + QString::number(emailMap[email]->port) );
            }
        } else {
            // if is not already online
            if (isOnline) {
                //> add to online map
                emailMap[email]->isOnline = isOnline;
                emailMap[email]->address = address;
                emailMap[email]->port = port;
                addrPortMap.insert(address.toString() + QString::number(port), emailMap[email]);
            }
            //> else do nothing
        }
        dataBaseLock.unlock();
        emit dataBaseChanged();
        return true;
    }
    return false;
}

bool DataBase::addMessageToLog(QString peer, QString message, bool isIncoming) {
    dataBaseLock.lock();

    // judge if toWho is in friends' list
    if (!emailMap.contains(peer))
        return false;
    emailMap[peer]->messageLog.append(MessageInfo(!isIncoming, message));

    dataBaseLock.unlock();
    return true;

    emit dataBaseChanged();
}

bool DataBase::addFileToLog(QString peer, QString filename, bool isIncoming) {
    dataBaseLock.lock();

    if (!emailMap.contains(peer))
        return false;
    emailMap[peer]->fileLog.append(MessageInfo(!isIncoming, filename));

    dataBaseLock.unlock();
    //TODO
}

void DataBase::serialize() {

}

void DataBase::deSerialize() {

}

DataBase::UserInfo::UserInfo() {
    isOnline = false;
    address = QHostAddress::Null;
    port = 0;
}

DataBase::MessageInfo::MessageInfo(bool isFromSelf, QString message) {
    this->isFromSelf = isFromSelf;
    this->message = message;
}
