/// @file database.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef DATABASE_H
#define DATABASE_H

#include "res.h"

#include <QObject>
#include <QString>
#include <QQueue>
#include <QMap>
#include <QPair>
#include <QHostAddress>
#include <QSharedPointer>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCoreApplication>

/// @brief the database to log the user's info for the client
/// although this class is somewhat resembles the DataBase class in
/// ServerCore, it's totally two different tings. The methods are different
/// due to the differece of the datastructure, caused by the diference
/// between the requirements of client and server.
///
/// this class is singleton and thread safe.
class DataBase : public QObject {
    Q_OBJECT
public:
    struct MessageInfo {
        /// @brief if the message is sent by myself
        bool isFromSelf;
        /// @brief the message content
        QString message;

        //TODO: timestamp
        MessageInfo(bool isFromSelf, QString message);
    };

    /// @brief the structure that stores the user info
    struct UserInfo {
        /// @brief the email/key of the user
        QString email;
        /// @brief the username
        QString username;
        /// @brief the avatar (bitmap format)
        QString avatar;

        /// @brief is the user currently online
        bool isOnline;
        /// @brief current ip address
        QHostAddress address;
        /// @brief current port
        quint16 port;

        /// @brief message log
        QQueue<MessageInfo> messageLog;
        /// @brief cached file
        QQueue<MessageInfo> fileLog;

        UserInfo();
        /// @brief does not allow copy
        UserInfo(const UserInfo &) = default;
        /// @brief does not allow copy
        UserInfo &operator =(const UserInfo &) = default;
    };

private:
    /// @brief constructor
    DataBase();

    /// @brief map email to user info
    QMap<QString, QSharedPointer<UserInfo>> emailMap;

    /// @brief map ip and port to user info
    /// @note only online user will be mapped
    QMap<QString, QSharedPointer<UserInfo>> addrPortMap;
    /// @brief thread safety lock
    QMutex dataBaseLock;

    /// @brief the path to the cache directory
    QString cachePath;

public:
    /// @brief get the singleton instance of the DataBase
    static DataBase &getInstance();

    /// @brief singleton assurance
    DataBase(const DataBase &) = delete;
    /// @brief singleton assurance
    DataBase &operator =(const DataBase &) = delete;

    /// @brief de-constructor
    ~DataBase() = default;

    /// @brief add user to dataBase
    /// @return if the adding successful (if not, the email is already in the map)
    bool addUser(QSharedPointer<UserInfo> user);

    /// @brief delete user from dataBase (seldom used)
    /// @return if the removal is successful (if not, the email is not in the map)
    bool delUser(QString email);

    /// @brief get user from database
    /// @return pointer to the userInfo, nullptr if none is found
    const UserInfo *getUser(QString email) const ;

    /// @brief get user from database by address and port
    /// @return pointer to the userInfo, nullptr if none is found
    /// only online user will be returned. check returnval.isNull() to check the return value
    const UserInfo *getUser(QHostAddress address, quint64 port) const;

    /// @brief get the list of users
    /// @return the userinfo list
    const QList<const UserInfo *> getUserList();

//    /// @brief modify user info
//    /// basically, this method updates user with key of email with
//    /// newUserInfo. the friend list, cached message and cached files are maintained
//    /// @return if the modification is successful
//    bool modifyUser(QString email, QSharedPointer<UserInfo> newUserInfo);

    /// @brief update use's online status (isOnline, ip, port)
    /// @return false if email is not in the database
    bool updateOnlineStatus(QString email, bool isOnline, QHostAddress address, quint16 port);

    /// @brief add a message to a user's message log
    bool addMessageToLog(QString peer, QString message, bool isIncoming);

    /// @brief add a file to a user's file log
    bool addFileToLog(QString peer, QString filename, bool isIncoming);

    /// @brief store data (messages record, info cache etc), append it to log file
    void serialize();

    /// @brief load data log (messages record, info cache, etc) from the file
    void deSerialize();

signals:

    void dataBaseChanged();
public slots:
};

#endif // DATABASE_H
