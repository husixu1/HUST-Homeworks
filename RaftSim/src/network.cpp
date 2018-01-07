/// @file network.cpp
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#include "network.h"

#include <QTextStream>
static QTextStream out(stdout);

Network::Network(QObject *parent) : QObject(parent) {
    propagationTime = 10;                   // 10 msec
    propagationTimeRandomized = false;
}

void Network::run(QMap<qint32, RaftNode *> nodes) {
    allNodes = nodes;
    for (auto id : nodes.keys())
        nodesStatus.insert(id, true);

    for (auto node : nodes.values()) {
        connect(node, SIGNAL(sendAppendEntryTo(qint32, QJsonObject)),
                this, SLOT(appendEntryIn(qint32, QJsonObject)));

        connect(node, SIGNAL(sendVoteRequestTo(qint32, QJsonObject)),
                this, SLOT(voteRequestIn(qint32, QJsonObject)));

        connect(node, SIGNAL(sendVoteTo(qint32, QJsonObject)),
                this, SLOT(voteRespondIn(qint32, QJsonObject)));
    }
}

void Network::setPropagationTime(qint32 time) {
    propagationTime = time;
}

void Network::takeNodeDown(qint32 nodeid) {
    if (nodesStatus.contains(nodeid))
        nodesStatus[nodeid] = false;
}

void Network::bringNodeUp(qint32 nodeid) {
    if (nodesStatus.contains(nodeid))
        nodesStatus[nodeid] = true;
}

void Network::setRandomizePropagationTime(bool state) {
    propagationTimeRandomized = state;
}

void Network::sendMessageTimedOut() {
    QTimer *timer = qobject_cast<QTimer *>(sender());
    Q_ASSERT(messages.contains(timer));
    Message message = messages[timer];

    // only send message to functional nodes
    if (nodesStatus[message.dstId] != false) {
        // filter data types
        switch (message.type) {
            case typeVoteRequest:
                out << "vote request " << message.data[QString(RaftNode::keyId)].toInt() << " -> " << message.dstId << endl;
                connect(this, SIGNAL(dataOut(QJsonObject)), allNodes[message.dstId], SLOT(receiveVoteRequest(QJsonObject)));
                break;
            case typeVoteRespond:
                out << "vote respond " << message.data[QString(RaftNode::keyId)].toInt() << " -> " << message.dstId << ", " << message.data["approve"].toBool() << endl;
                connect(this, SIGNAL(dataOut(QJsonObject)), allNodes[message.dstId], SLOT(receiveVoteResponse(QJsonObject)));
                break;
            case typeAppendEntry:
                out << "appendEntry " << message.data[QString(RaftNode::keyId)].toInt() << " -> " << message.dstId << endl;
                connect(this, SIGNAL(dataOut(QJsonObject)), allNodes[message.dstId], SLOT(receiveAppendEntry(QJsonObject)));
                break;
        }
        emit dataOut(message.data);
        disconnect(this, SIGNAL(dataOut(QJsonObject)), 0, 0);
    }

    // remove from map
    messages.remove(timer);
    delete timer;
}

void Network::voteRequestIn(qint32 dstId, QJsonObject data) {
    // broken nodes cannot send messages
    if (nodesStatus[data["id"].toInt()] == false)
        return;
    addToMessageList(data, typeVoteRequest, dstId);
}

void Network::voteRespondIn(qint32 dstId, QJsonObject data) {
    // broken nodes cannot send messages
    if (nodesStatus[data["id"].toInt()] == false)
        return;
    addToMessageList(data, typeVoteRespond, dstId);
}

void Network::appendEntryIn(qint32 dstId, QJsonObject data) {
    // broken nodes cannot send messages
    if (nodesStatus[data["id"].toInt()] == false)
        return;
    addToMessageList(data, typeAppendEntry, dstId);
}

void Network::addToMessageList(QJsonObject data, Network::MessageType type, qint32 dstId) {
    if (allNodes.contains(dstId)) {
        Message msg;
        msg.type = type;
        msg.dstId = dstId;
        msg.data = data;

        msg.timer = new QTimer;
        msg.timer->setSingleShot(true);
        connect(msg.timer, SIGNAL(timeout()), this, SLOT(sendMessageTimedOut()));

        messages.insert(msg.timer, msg);
        msg.timer->start(propagationTime / 2 + (propagationTimeRandomized ? (rand() % propagationTime) : (propagationTime / 2)));
    }
}
