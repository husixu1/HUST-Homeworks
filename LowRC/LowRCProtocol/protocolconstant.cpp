/// @file protocolconstant.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "protocolconstant.h"

const QMap<ProtocolConstant::ProtocolKeyType, QString> ProtocolConstant::keyMap = {
    {ProtocolConstant::keyReply,        "reply"},
    {ProtocolConstant::keyEmail,        "email"},
    {ProtocolConstant::keyUserName,     "username"},
    {ProtocolConstant::keyPasswd,       "passwd"},
    {ProtocolConstant::keyAvatar,       "avatar"},
    {ProtocolConstant::keySafeQuestion, "question"},
    {ProtocolConstant::keySafeAnswer,   "answer"},
    {ProtocolConstant::keyAddress,      "address"},
    {ProtocolConstant::keyPort,         "port"},
    {ProtocolConstant::keyTargetEmail,  "target"},
    {ProtocolConstant::keyFriend,       "friend"},
    {ProtocolConstant::keyMessage,      "message"},
    {ProtocolConstant::keyFile,         "file"},
    {ProtocolConstant::keyIsOnline,     "isonline"},
    {ProtocolConstant::keyValidate,     "validate"},
    {ProtocolConstant::keyFileName,     "filename"},
    {ProtocolConstant::keyFileSize,     "filesize"},
    {ProtocolConstant::keyIdentifier,   "identifier"},
};

const QMap<ProtocolConstant::ReplyValueType, QString> ProtocolConstant::replyValueMap = {
    {ProtocolConstant::replyOK,                    "ok"},
    {ProtocolConstant::replyIncorrectPassword,     "incorrectPassword"},
    {ProtocolConstant::replyAlreadyRegistered,     "alreadyRegistered"},
    {ProtocolConstant::replyUserNotRegistered,     "userNotRegistered"},
    {ProtocolConstant::replyWrongAnswer,           "wrongAnswer"},
    {ProtocolConstant::replyInvalidRequest,        "invalidRequest"},
    {ProtocolConstant::replyTargetNotRegistered,   "targetNotRegistered"},
    {ProtocolConstant::replyUserAlreadyOnline,     "userAlreadyOnline"},
};



