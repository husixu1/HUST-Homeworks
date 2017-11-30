/// @file dataprocessor.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "dataprocessor.h"

#include "baseconnector.h"
#include "database.h"

#include <QtDebug>

const QString DataProcessor::keyConnectAddress = QString("__connectedAddress");
const QString DataProcessor::keyConnectPort = QString("__connectedPort");

namespace PC = ProtocolConstant;

void DataProcessor::dataSent() {
    //> get the operation
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());

    //> get the request type and json (maybe modified, added PC::keys are started with _)
    BaseConnector::DataType requestType = operationMap[operation].first;
    QJsonObject infoJson = operationMap[operation].second;

    // deal with data
    switch (requestType) {
        case BaseConnector::typeRegisterRequest: {
            //> create and fill a userinfo
            QSharedPointer<DataBase::UserInfo> userInfo(new DataBase::UserInfo);
            userInfo->email = infoJson[PC::keyMap[PC::keyEmail]].toString();
            userInfo->passwd = infoJson[PC::keyMap[PC::keyPasswd]].toString();
            userInfo->username = infoJson[PC::keyMap[PC::keyUserName]].toString();
            userInfo->avatar = infoJson[PC::keyMap[PC::keyAvatar]].toString();
            userInfo->safeQuestion = infoJson[PC::keyMap[PC::keySafeQuestion]].toString();
            userInfo->safeQuestionAnswer = infoJson[PC::keyMap[PC::keySafeAnswer]].toString();
            //> add to database
            DataBase::getInstance().addUser(userInfo);
            //TODO: update online status
            qDebug().noquote() << "user \"" << userInfo->email << "\" added" << endl;
            break;
        }
        case BaseConnector::typeLoginRequest: {
            //> set the user's online status if send is successful
            DataBase::getInstance().updateOnlineStatus(
                    infoJson[PC::keyMap[PC::keyEmail]].toString(), true,
                    QHostAddress(infoJson[keyConnectAddress].toString()),
                    static_cast<quint16>(infoJson[keyConnectPort].toInt()),
                    static_cast<quint16>(infoJson[PC::keyMap[PC::keyPort]].toInt()));

            //> clear the user's cached messages and files if send successful
            DataBase::getInstance().clearCachedFileAndEmail(infoJson[PC::keyMap[PC::keyEmail]].toString());

            //TODO: remove cached files, messages, etc
            qDebug().noquote() << "user " << infoJson[PC::keyMap[PC::keyEmail]].toString()
                    << " logged in at " << infoJson[keyConnectAddress].toString()
                    << "@" << infoJson[keyConnectPort].toString()
                    << "and listening at " << infoJson[keyConnectAddress].toString()
                    << "@" << infoJson[PC::keyMap[PC::keyPort]].toString() << endl;

            //> broadcast this user's online message to all online users
            //>> construct the broadcase send object (note the port here is the peer's listening port)
            QJsonObject sendObject;
            sendObject.insert(PC::keyMap[PC::keyEmail], infoJson[PC::keyMap[PC::keyEmail]].toString());
            sendObject.insert(PC::keyMap[PC::keyIsOnline], true);
            sendObject.insert(PC::keyMap[PC::keyAddress], infoJson[keyConnectAddress].toString());
            sendObject.insert(PC::keyMap[PC::keyPort], infoJson[PC::keyMap[PC::keyPort]].toInt());

            //>> broadcast to all online user
            for (auto i : DataBase::getInstance().getOnlineUserList()) {
                QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
                emit sendData(operation, i->address, i->connectedPort,
                        operation->getId(), BaseConnector::typeUserBroadcast,
                        QJsonDocument(sendObject).toBinaryData());
            }
            break;
        }
        case BaseConnector::typePassword1Request: {
            Q_ASSERT(false); // function should not reach here
            break;
        }
        case BaseConnector::typePassword2Request:
            Q_ASSERT(false); // function should not reach here
            break;
        case BaseConnector::typePassword3Request: {
            const DataBase::UserInfo *oldUserInfo = DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString());
            QSharedPointer<DataBase::UserInfo> newUserInfo(new DataBase::UserInfo(*oldUserInfo));
            //> set the user's passwd and store to database
            newUserInfo->passwd = infoJson[PC::keyMap[PC::keyPasswd]].toString();
            DataBase::getInstance().modifyUser(infoJson[PC::keyMap[PC::keyEmail]].toString(), newUserInfo);
            qDebug().noquote() << "user \"" << oldUserInfo->email << "\" changed passwd to " << newUserInfo->passwd;
            break;
        }
        case BaseConnector::typeMessageRequest: {
            //> cache message for target
            // TODO: return if successful
            DataBase::getInstance().cacheMessage(
                    infoJson[PC::keyMap[PC::keyEmail]].toString(),
                    infoJson[PC::keyMap[PC::keyTargetEmail]].toString(),
                    infoJson[PC::keyMap[PC::keyMessage]].toString() );
            qDebug().noquote() << "user \"" << infoJson[PC::keyMap[PC::keyEmail]].toString()
                    << "\" cached a message to " << infoJson[PC::keyMap[PC::keyTargetEmail]].toString() << endl;
            break;
        }
        case BaseConnector::typeFileRequest:
            break;
        default:
            break;
    }
    // remove from map
    operationPool.remove(operation);
    operationMap.remove(operation);
    // TODO: log
}

void DataProcessor::failedToSend(OperationDescriptor::Error) {
    //> get the operation
    OperationDescriptor *operation = qobject_cast<OperationDescriptor *>(sender());
    qDebug().noquote() << "failed to send data, operationID=" << operation->getId() << endl;

    //> do no opeartion remove from map
    operationPool.remove(operation);
    operationMap.remove(operation);
    // TODO: log
}

void DataProcessor::processData(QHostAddress address, quint16 port, QByteArray data) {
    QByteArray resolvedData;
    BaseConnector::DataType type;
    quint64 reqNum;

    //> resolve the data
    BaseConnector::resolveData(data, type, reqNum, resolvedData);

    //> convert to json
    QJsonObject infoJson = QJsonDocument::fromBinaryData(resolvedData).object();
    QJsonObject replyJson;

    qDebug().noquote() << "got request: type " << static_cast<quint32>(type) << ": " << endl
            << "address " << address.toString() << ": port " << port << ": reqNum " << reqNum << endl
            << QJsonDocument(infoJson).toJson(QJsonDocument::Indented) << endl;

    //> perpare an operation and connect signals
    QSharedPointer<OperationDescriptor> operation = OperationDescriptor::getNewOperationDescriptor();
    connect(operation.data(), SIGNAL(dataSent()),
            this, SLOT(dataSent()));
    connect(operation.data(), SIGNAL(sendReceiveError(OperationDescriptor::Error)),
            this, SLOT(failedToSend(OperationDescriptor::Error)));

    //> act according to different request types
    switch (type) {
        case BaseConnector::typeRegisterRequest: {
            // send reply messages
            if (DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString()) ) {
                //> do not have to perform operatoins
                operation.data()->disconnect();

                //> user is already in the database, reply already registered
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyAlreadyRegistered]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeRegisterAnswer, QJsonDocument(replyJson).toBinaryData());
            } else {
                //> insert to pool
                operationPool.insert(operation.data(), operation);
                operationMap.insert(operation.data(), {type, infoJson});

                //> user not registered, reply
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeRegisterAnswer, QJsonDocument(replyJson).toBinaryData());
            }
            break;
        }
        case BaseConnector::typeLoginRequest: {
            // judge if the passwd is correct
            const DataBase::UserInfo *user = DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString());
            if (!user) {
                operation.data()->disconnect();
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyUserNotRegistered]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeLoginAnswer, QJsonDocument(replyJson).toBinaryData());
                break;
            }

            QString passwd = user->passwd;
            if (passwd == infoJson[PC::keyMap[PC::keyPasswd]].toString()) {
                //> if user is already online
                if (user->isOnline) {
                    operation.data()->disconnect();
                    replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyUserAlreadyOnline]);
                    emit sendData(operation, address, port, reqNum,
                            BaseConnector::typeLoginAnswer, QJsonDocument(replyJson).toBinaryData());
                }

                //> add address and port to info to data && insert data to process map
                infoJson.insert(keyConnectAddress, address.toString());
                infoJson.insert(keyConnectPort, port);
                operationPool.insert(operation.data(), operation);
                operationMap.insert(operation.data(), {type, infoJson});

                //> if the passwd is correct, send ok
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);

                const DataBase::UserInfo *user = DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString());
                //> initialize reply json
                //>> initialize friendlist
                QJsonObject friendList;

//                // FIXME: now assuming everyone is everyone's friend for experiment check, fix that to friends!
//                for (QString friendEmail : user->friends) {
//                    QJsonObject status;
//                    //> add friend's status to the friend email
//                    const DataBase::UserInfo *userInfo = DataBase::getInstance().getUser(friendEmail);
//                    status.insert(PC::keyMap[PC::keyIsOnline], userInfo->isOnline);
//                    status.insert(PC::keyMap[PC::keyAddress], userInfo->address.toString());
//                    status.insert(PC::keyMap[PC::keyPort], userInfo->listenPort);
//                    friendList.insert(friendEmail, status);
//                }

                for (auto fakeFriends : DataBase::getInstance().getUserList()) {
                    //> skip self, do not have to do this in real case
                    if (fakeFriends->email == infoJson[PC::keyMap[PC::keyEmail]].toString())
                        continue;

                    //> add user status
                    QJsonObject status;
                    status.insert(PC::keyMap[PC::keyUserName], fakeFriends->username);
                    status.insert(PC::keyMap[PC::keyIsOnline], fakeFriends->isOnline);
                    status.insert(PC::keyMap[PC::keyAddress], fakeFriends->address.toString());
                    status.insert(PC::keyMap[PC::keyPort], fakeFriends->listenPort);
                    status.insert(PC::keyMap[PC::keyAvatar], fakeFriends->avatar);
                    friendList.insert(fakeFriends->email, status);
                }

                //>> initialize cached message list
                QJsonObject cachedMessage;
                for (auto messages : user->cachedMessage) {
                    //> append messages to corresponding user
                    if (cachedMessage.contains(messages.first)) {
                        QJsonArray temp = cachedMessage[messages.first].toArray();
                        temp.append(messages.second);
                        cachedMessage[messages.first] = temp;
                    } else
                        cachedMessage.insert(messages.first, QJsonArray({messages.second}));
                }

                //>> initialize cached file list
                QJsonObject cachedFile;
                for (auto file : user->cachedFile) {
                    //> append files to corresponding user
                    if (cachedFile.contains(file.first)) {
                        QJsonArray temp = cachedFile[file.first].toArray();
                        temp.append(file.second);
                        cachedFile[file.first] = temp;
                    } else
                        cachedFile.insert(file.first, QJsonArray({file.second}));
                }

                //> add datas to friend list
                replyJson.insert(PC::keyMap[PC::keyFriend], friendList);
                replyJson.insert(PC::keyMap[PC::keyMessage], cachedMessage);
                replyJson.insert(PC::keyMap[PC::keyFile], cachedFile);

                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeLoginAnswer, QJsonDocument(replyJson).toBinaryData());
            } else {
                //> do not have to perform operatoins
                operation.data()->disconnect();

                //> if the passwd is incorrect, send incorrect passwd
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyIncorrectPassword]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeLoginAnswer, QJsonDocument(replyJson).toBinaryData());
            }
            break;
        }
        case BaseConnector::typePassword1Request: {
            //> do not have to perform operatoins
            operation.data()->disconnect();

            if (DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString())) {
                //> if the user is found, send ok and question
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);
                replyJson.insert(PC::keyMap[PC::keySafeQuestion],
                        DataBase::getInstance().getUser(
                                infoJson[PC::keyMap[PC::keyEmail]].toString())->safeQuestion);

                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typePassword1Answer, QJsonDocument(replyJson).toBinaryData());
            } else {
                //> if the user is not found, send uesrNotFound
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyUserNotRegistered]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typePassword1Answer, QJsonDocument(replyJson).toBinaryData());
            }
            break;
        }
        case BaseConnector::typePassword2Request: {
            //> do not have to perform operations
            operation.data()->disconnect();

            //> get the user
            const DataBase::UserInfo *user = DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString());
            if (user) {
                // if the user is found, compare the answer
                if (infoJson[PC::keyMap[PC::keySafeAnswer]].toString() == user->safeQuestionAnswer) {
                    //> generate a random validate PC::key
                    std::srand(std::time(0));
                    int validateNumber = std::rand();

                    //> add to pending validate map and add a timeout timer
                    QSharedPointer<QTimer> timer(new QTimer);
                    validateMap.insert(infoJson[PC::keyMap[PC::keyEmail]].toString() + QString::number(validateNumber), timer);
                    validateTimerMap.insert(timer.data(), infoJson[PC::keyMap[PC::keyEmail]].toString() + QString::number(validateNumber));
                    timer->start(validateTimeout);

                    //> if the answer is correct, send ok
                    replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);
                    replyJson.insert(PC::keyMap[PC::keyValidate], validateNumber);
                    emit sendData(operation, address, port, reqNum,
                            BaseConnector::typePassword2Answer, QJsonDocument(replyJson).toBinaryData());
                } else {
                    //> if the answer is incorrect, send wrongAnswer
                    infoJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyWrongAnswer]);
                    emit sendData(operation, address, port, reqNum,
                            BaseConnector::typePassword2Answer, QJsonDocument(replyJson).toBinaryData());
                }
            } else {
                //> if the user is not found, send userNotFound
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyUserNotRegistered]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typePassword1Answer, QJsonDocument(replyJson).toBinaryData());
            }
            break;
        }
        case BaseConnector::typePassword3Request: {
            const DataBase::UserInfo *user = DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString());
            if (user) {
                // if this is a valid request
                if (validateMap.contains(infoJson[PC::keyMap[PC::keyEmail]].toString()
                                + QString::number(infoJson[PC::keyMap[PC::keyValidate]].toInt())) ) {
                    //> add to operationPool to change the passwd
                    operationPool.insert(operation.data(), operation);
                    operationMap.insert(operation.data(), {type, infoJson});

                    //> if the request is valid, pending to change passwd and send ok
                    replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);
                    emit sendData(operation, address, port, reqNum,
                            BaseConnector::typePassword3Answer, QJsonDocument(replyJson).toBinaryData());
                } else {
                    //> do not have to perform operations
                    operation.data()->disconnect();
                    //> send not valid
                    replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyInvalidRequest]);
                    emit sendData(operation, address, port, reqNum,
                            BaseConnector::typePassword3Answer, QJsonDocument(replyJson).toBinaryData());
                }
            } else {
                //> do not have to perform operations
                operation.data()->disconnect();
                //> if the user is not registered, reply userNotRegistered
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyUserNotRegistered]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typePassword3Answer, QJsonDocument(replyJson).toBinaryData());
            }
            break;
        }
        case BaseConnector::typeMessageRequest: {
            const DataBase::UserInfo *user =
                    DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyEmail]].toString());
            const DataBase::UserInfo *target =
                    DataBase::getInstance().getUser(infoJson[PC::keyMap[PC::keyTargetEmail]].toString());
            if (user) {
                // if target user is not found
                if (!target) {
                    //> do not have to perform operations
                    operation.data()->disconnect();
                    replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyTargetNotRegistered]);
                    emit sendData(operation, address, port, reqNum,
                            BaseConnector::typeMessageAnswer, QJsonDocument(replyJson).toBinaryData());
                }
                //> if the user is found, pend data to process
                operationPool.insert(operation.data(), operation);
                operationMap.insert(operation.data(), {type, infoJson});
                //> reply ok
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyOK]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeMessageAnswer, QJsonDocument(replyJson).toBinaryData());
            } else {
                //> do not have to perform operations
                operation.data()->disconnect();
                // if the user is not found
                replyJson.insert(PC::keyMap[PC::keyReply], PC::replyValueMap[PC::replyUserNotRegistered]);
                emit sendData(operation, address, port, reqNum,
                        BaseConnector::typeMessageAnswer, QJsonDocument(replyJson).toBinaryData());
            }
            break;
        }
        case BaseConnector::typeFileRequest: {
            break;
        }
        default:
            break;
    }
}

void DataProcessor::unexpectedError(QHostAddress address, quint16 connectPort, BaseConnector::UnexpectedError) {
    qDebug().noquote() << "client disconnected: " << address.toString() << QString::number(connectPort) << endl;

    //> change the user status to offline (if user is registered)
    const DataBase::UserInfo *user;
    if ((user = DataBase::getInstance().getUser(address, connectPort))) {
        DataBase::getInstance().updateOnlineStatus(
                DataBase::getInstance().getUser(address, connectPort)->email,
                false, QHostAddress::Null, 0, 0);
        qDebug().noquote() << "user " << user->email << " logged out" << endl;
    }
    // TODO: broadcast
}
