/// @file protocolconstant.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef PROTOCOLCONSTANT_H
#define PROTOCOLCONSTANT_H

#include <QString>
#include <QMap>

namespace ProtocolConstant {

enum ProtocolKeyType {
    keyReply,
    keyEmail,
    keyUserName,
    keyPasswd,
    keyAvatar,
    keySafeQuestion,
    keySafeAnswer,
    keyAddress,
    keyPort,
    keyTargetEmail,
    keyFriend,
    keyMessage,
    keyFile,
    keyIsOnline,
    keyValidate,
    keyFileName,
    keyFileSize,
    keyIdentifier,
};

/// @brief the possible keys in protocol
extern const QMap <ProtocolKeyType, QString> keyMap;

/// @brief the server reply value
/// reply value are a group of specified vaule
/// indicating the server's data process status
enum ReplyValueType {
    replyOK,
    replyAlreadyRegistered,
    replyIncorrectPassword,
    replyUserNotRegistered,
    replyWrongAnswer,
    replyInvalidRequest,
    replyTargetNotRegistered,
    replyUserAlreadyOnline,
};

/// @brief maps server reply types
extern const QMap<ReplyValueType, QString> replyValueMap;
}

#endif // PROTOCOLCONSTANT_H
