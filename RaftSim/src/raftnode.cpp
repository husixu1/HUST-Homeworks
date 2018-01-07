/// @file raftnode.cpp
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#include "raftnode.h"
#include <QDebug>

const quint32 RaftNode::baseElectionTimeout = 10;
const quint32 RaftNode::baseAppendEntryTimeout = 3;

static const qint32 invalid = -1;

RaftNode::State RaftNode::getState() const {
    return state;
}

void RaftNode::resetTimeout() {
    electionTimeOut = rand() % baseElectionTimeout + baseElectionTimeout / 2;
    elapsedTime = 0;
}

void RaftNode::startElection() {
    sendInfo({{keyId, id}, {keyEvent, eventElectionStarted}});

    // update current term
    setCurrentTerm(currentTerm + 1);

    // clear vote status and vote for myself
    for (auto &node : allNodes.values())
        node.votedForMe = false;
    voteFor(id, true);
    currentLeader = invalid;

    // become a candidate
    state = stateCandidate;
    sendInfo({{keyId, id}, {keyEvent, eventBecameCandidate}});

    // send to all nodes except myself
    for (auto &node : allNodes.values())
        if (node.id != id)
            emit requestVoteFrom(node.id);

    // randomize election timeout to avoid forever loop
    resetTimeout();
}

void RaftNode::sendInfo(QList<QPair<InfoKey, QJsonValue> > infoList) {
    QJsonObject message;
    for (auto pair : infoList)
        message.insert(QString(pair.first), pair.second);
    emit info(message);
}

bool RaftNode::shouldVote(RaftNode::VoteRequest request) {
    // not vote for nodes whoes term is less than this node
    if (request.term < currentTerm)
        return false;

    // one vote a term
    if (votedTo != invalid)
        return false;

    // TODO: log entry judgements ...
    return true;
}

void RaftNode::setCurrentTerm(qint64 newTerm) {
    if (currentTerm < newTerm) {
        currentTerm = newTerm;
        votedTo = invalid;
    }
}

void RaftNode::routine() {
    ++elapsedTime;

    // only this node alone, no problem to become a leader
    if (allNodes.size() == 1 && state != stateLeader)
        state = stateLeader;

    if (state == stateLeader) {
        // if this node is leader
        if  (elapsedTime > appendEntryTimeout)
            broadcastAppendEntry();
    } else {
        // if this node is not leader and timed out
        if (elapsedTime > electionTimeOut)
            startElection();
    }
}

void RaftNode::voteFor(qint32 dstId, bool approve) {
    // if vote for myself
    if (dstId == id && approve) {
        allNodes[id].votedForMe = true;
        votedTo = dstId;
        return;
    }

    // vote for other nodes
    if (approve)
        votedTo = dstId;

    VoteResponse response;
    response.id = id;
    response.approve = approve;
    response.term = currentTerm;
    emit sendVoteTo(dstId, response.toJson());
}

void RaftNode::requestVoteFrom(qint32 dstId) {
    VoteRequest request;
    request.id = id;
    request.term = currentTerm;
    emit sendVoteRequestTo(dstId, request.toJson());
}

void RaftNode::broadcastAppendEntry() {
    AppendEntry appendEntry;
    resetTimeout();
    for (auto node : allNodes) {
        if (node.id != id) {
            appendEntry.id = id;
            appendEntry.term = currentTerm;
            emit sendAppendEntryTo(node.id, appendEntry.toJson());
        }
    }
}

RaftNode::RaftNode(QObject *parent) : QObject(parent) {
    // set initial value
    currentLeader = invalid;
    currentTerm = 0;
    votedTo = invalid;
    state = stateFollower;
    appendEntryTimeout = baseAppendEntryTimeout;
    electionTimeOut = baseElectionTimeout;
    elapsedTime = 0;
    started = false;

    // connect timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(routine()));
}

void RaftNode::run(QList<qint32> allNodesId, qint32 myId, qint32 speed) {
    // cannot be started twice (unresonable)
    if (started)
        return;

    Node temp;
    for (auto node : allNodesId) {
        temp.id = node;
        temp.votedForMe = false;
        allNodes.insert(node, temp);
    }
    id = myId;

    // start this node
    timer.start(speed);
    started = true;
}

void RaftNode::setTimerSpeed(qint32 speed) {
    timer.start(speed);
}

qint32 RaftNode::getId() {
    return id;
}

void RaftNode::receiveVoteRequest(QJsonObject voteRequest) {
    VoteRequest request = VoteRequest::fromJson(voteRequest);

    // if my term < request node's term, become a follower unconditionally
    if (currentTerm < request.term) {
        setCurrentTerm(request.term);
        if (state != stateFollower) {
            state = stateFollower;
            sendInfo({{keyId, id}, {keyEvent, eventBecameFollower}});
        }

        resetTimeout();
    }

    if (shouldVote(request)) {
        voteFor(request.id, true);
        currentLeader = invalid;
        resetTimeout();
    } else
        voteFor(request.id, false);

    resetTimeout();
}

void RaftNode::receiveVoteResponse(QJsonObject voteResponse) {
    VoteResponse response = VoteResponse::fromJson(voteResponse);
    // if this node is not a candidate
    if (state != stateCandidate)
        return;

    // become a follower if this node's term is not the max
    if (response.term > currentTerm) {
        setCurrentTerm(response.term);
        if (state != stateFollower) {
            state = stateFollower;
            sendInfo({{keyId, id}, {keyEvent, eventBecameFollower}});
        }

        // reset election timeout
        resetTimeout();
        return;
    }

    // ignore outdated message
    if (response.term < currentTerm)
        return;

    // ignore unknown nodes
    if (!allNodes.contains(response.id))
        return;

    if (response.approve) {
        // count votes if approved
        allNodes[response.id].votedForMe = true;

        qint32 totalVotes = 0;
        for (auto &node : allNodes)
            totalVotes += node.votedForMe ? 1 : 0;

        sendInfo({ {keyId, id},
            {keyEvent, eventReceivedResponse},
            {keyVoteNum, totalVotes}
        });

        // become leader if gets majority votes
        qInfo() << "--- total vote num" << totalVotes;
        if (totalVotes >= allNodes.size() / 2 + 1) {
            state = stateLeader;
            currentLeader = id;
            sendInfo({{keyId, id}, {keyEvent, eventBecameLeader}});
            resetTimeout();
        }
    } else {
        // not approved
        allNodes[response.id].votedForMe = false;
    }

}

void RaftNode::receiveAppendEntry(QJsonObject appendEntry) {
    AppendEntry entry = AppendEntry::fromJson(appendEntry);

    // TEST: ignore append entry when i'm leader
    if (state == stateLeader)
        return;

    // judging by entry term and my term
    if (state == stateCandidate && currentTerm == entry.term) {
        state = stateFollower;
        sendInfo({{keyId, id}, {keyEvent, eventBecameFollower}});
    } else if (currentTerm < entry.term) {
        setCurrentTerm(entry.term);
        if (state != stateFollower) {
            state = stateFollower;
            sendInfo({{keyId, id}, {keyEvent, eventBecameFollower}});
        }
    } else if (currentTerm > entry.term) {
        // TODO: eliminate that monarch and become the king of the new era!
        // currently just wait for timeout
        return;
    }
    currentLeader = entry.id;
    resetTimeout();
}

QJsonObject RaftNode::VoteRequest::toJson() {
    QJsonObject json;
    json.insert("id", id);
    json.insert("term", term);
    json.insert("lastLogIndex", lastLogIndex);
    json.insert("lastLogTerm", lastLogTerm);
    return json;
}

RaftNode::VoteRequest RaftNode::VoteRequest::fromJson(QJsonObject json) {
    VoteRequest result;
    result.id = json["id"].toInt();
    result.term = json["term"].toInt();
    result.lastLogIndex = json["lastLogIndex"].toInt();
    result.lastLogTerm = json["lastLogTerm"].toInt();
    return result;
}


QJsonObject RaftNode::VoteResponse::toJson() {
    QJsonObject json;
    json.insert("id", id);
    json.insert("term", term);
    json.insert("approve", approve);
    return json;
}

RaftNode::VoteResponse RaftNode::VoteResponse::fromJson(QJsonObject json) {
    VoteResponse result;
    result.id = json["id"].toInt();
    result.term = json["term"].toInt();
    result.approve = json["approve"].toBool();
    return result;
}

QJsonObject RaftNode::AppendEntry::toJson() {
    QJsonObject json;
    json.insert("id", id);
    json.insert("term", term);
    return json;
}

RaftNode::AppendEntry RaftNode::AppendEntry::fromJson(QJsonObject json) {
    AppendEntry result;
    result.id = json["id"].toInt();
    result.term = json["term"].toInt();
    return result;
}
