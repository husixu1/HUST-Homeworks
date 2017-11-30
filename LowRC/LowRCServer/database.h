/// @file database.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef DATABASE_H
#define DATABASE_H

#include "res.h"
#include "AES.h"

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

/// @brief the database to store users' info
class DataBase : public QObject {
    Q_OBJECT
public:
    /// @brief the structure that stores the user info
    struct UserInfo {
        /// @brief the email/key of the user
        QString email;
        /// @brief the username
        QString username;
        /// @brief the passwd (encrypted)
        QString passwd;
        /// @brief the avatar (bitmap format)
        QString avatar;
        /// @brief safe question
        QString safeQuestion;
        /// @brief answer to safe question
        QString safeQuestionAnswer;
        /// @brief friend list
        QSet<QString> friends;

        /// @brief is the user currently online
        volatile bool isOnline;
        /// @brief current ip address
        QHostAddress address;
        /// @brief the users LISTENING port
        /// @note this is the user's current listening port (if user is online),
        /// not the connected port. server should not send message through this port,
        /// only forward this port number to other users
        quint16 listenPort;

        /// @brief the port this client is used to connect to the server
        /// @note this port will be constantly updated by the connector
        volatile quint16 connectedPort;

        /// @brief cached message
        QQueue<QPair<QString, QString>> cachedMessage;
        /// @brief cached file
        QQueue<QPair<QString, QString>> cachedFile;

        /// @brief constructor, set default values
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

    /// @brief map online user's email to user info
    /// @note only online user will be mapped.
    /// @note the port is the user's CONNECTED port used to connect to this host,
    /// not the listening port, the listening port should be got from the UserInfo struct.
    QMap<QString, QSharedPointer<UserInfo>> onlineMap;

    /// @brief map online user's ip and connectPort to the user info
    /// this map is used to find user by address and connectPort (the port that
    /// the user used to connect this server)
    QMap<QString, QSharedPointer<UserInfo>> onlineAddressPortMap;

    /// @brief the lock to lock the database from changing
    QMutex dataBaseLock;

    /// @brief the path to the database
    /// @note if not abosolute, the path will be relative to the dir containing this app executable
    QString dataBasePath;

    uint8_t *encryptKey;

public:
    /// @brief get the singleton instance of the DataBase
    static DataBase &getInstance();

    /// @brief singleton assurance
    DataBase(const DataBase &) = delete;
    /// @brief singleton assurance
    DataBase &operator =(const DataBase &) = delete;

    /// @brief de-constructor
    ~DataBase();

    /// @brief encrypt data
    /// data does not have to be 16 bytes padded.
    /// assume key is 16 bytes
    /// @return the encrypted bytes
    static QByteArray AesEncrypt(QByteArray data, const uint8_t *key) ;

    /// @brief decrypt data
    /// assume data is 16 bytes padded.
    /// assume key is 16 bytes
    /// @return the decypted bytes
    static QByteArray AesDecrypt( QByteArray data, const uint8_t *key);

public:
    /// @brief add user to dataBase
    /// @return if the adding successful (if not, the email is already in the map)
    bool addUser(QSharedPointer<UserInfo> user);

    /// @brief delete user from dataBase (seldom used)
    /// @return if the removal is successful (if not, the email is not in the map)
    bool delUser(QString email);

    /// @brief get user from database
    /// @return pointer to the userInfo, nullptr if none is found
    const UserInfo *getUser(QString email) const ;

    /// @brief get user from database by address and connectPort
    /// @return pointer to the userInfo, nullptr if none is found
    /// only online user will be returned. check returnval.isNull() to check the return value
    const UserInfo *getUser(QHostAddress address, quint64 connectPort) const;

    /// @brief get the list of users
    /// @return the userinfo list
    const QList<const UserInfo *> getUserList();

    /// @brief get the list of online users
    /// @return the online user list
    const QList<const UserInfo *> getOnlineUserList();

    /// @brief modify user info
    /// basically, this method updates user with key of email with
    /// newUserInfo. the friend list, cached message and cached files are maintained
    /// @return if the modification is successful
    bool modifyUser(QString email, QSharedPointer<UserInfo> newUserInfo);

    /// @brief update use's online status (isOnline, ip, connectedPort)
    /// @param[in] email the user's email
    /// @param[in] isOnline should this user be online or not
    /// @param[in] address the user's address, ignored if isOnline is false
    /// @param[in] connectPort the user's port connected to this server (not the listening port), ignored if isOnline is false
    /// @param[in] listenPort the user's listening port, ignored if isOnline is false
    /// @return false if email is not in the database
    bool updateOnlineStatus(QString email, bool isOnline,
            QHostAddress address, quint16 connectPort,  quint16 listenPort);

    /// @brief cache a message for a user
    /// @return false if toWho is not in database
    bool cacheMessage(QString fromWho, QString toWho, QString message);

    /// @brief cache a file for a user
    bool cacheFile(QString fromWho, QString toWho, QString filename);

    /// @brief clear the cached file and email
    /// @return false if user not found, true otherwise
    bool clearCachedFileAndEmail(QString userEmail);

    /// @brief serialize data to disk
    bool serialize();

    /// @brief deSerialize data from disk
    /// @note this function will only update the not-existing users
    /// so it should only be used when starting up the server.
    /// make sure the encryption function is called before this function called
    bool deSerialize();

    /// @brief set the encryption key
    /// assume value is 16 bytes, exceeded bytes will be truncated.
    /// this function will perform a deep copy
    void setEncryptKey(const uint8_t *value);

signals:
    /// @brief signal emitted when databsae is changed
    /// @note UI should use queued connection as the dataBase is in another thread
    void dataBaseChanged();
};

#endif // DATABASE_H

// TODO: add doxygen more for all things
