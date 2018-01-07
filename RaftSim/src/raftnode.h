/// @file raftnode.h
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#ifndef RAFTNODE_H
#define RAFTNODE_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>
#include <QList>
#include <QPair>
#include <functional>

/// @brief Each implementation of a RaftNode class represents
/// a server running raft protocol
class RaftNode : public QObject {
    Q_OBJECT
public:
    /// @brief json keys in emitted signal 'info', use with info[QString(<keyName>)]
    enum InfoKey {
        keyId,                  ///< @brief emitter's id
        keyEvent,               ///< @brief type of the event (what happened)
        keyVoteNum,             ///< @brief vote count
    };
    /// @brief values for json key 'event' in emitted signal 'info'
    enum InfoEvent {
        eventElectionStarted,   ///< @brief this node started an election
        eventBecameCandidate,   ///< @brief this node has became a candidate
        eventBecameFollower,    ///< @brief this node has became a follower
        eventBecameLeader,      ///< @brief this node has became a leader
        eventReceivedResponse,  ///< @brief received a vote response from other node
    };

private:
    /// @brief node's 3 possible state: follower/candidate/leader
    enum State {
        stateFollower,          ///< @brief follower state
        stateCandidate,         ///< @brief candidate state
        stateLeader,            ///< @brief leader state
    };

    /// @brief the vote request to send to other nodes
    struct VoteRequest {
        qint64 term;            ///< @brief sender's term
        qint32 id;              ///< @brief sender's id
        qint32 lastLogIndex;    ///< @brief sender's last log's index, FIXME: not used
        qint32 lastLogTerm;     ///< @brief sender's last log's term, FIXME: not used

        /// @brief convert to json
        /// @return the converted json object
        QJsonObject toJson();
        /// @brief convert from json
        /// @param json the json object to convert
        /// @return the constructed VoteRequest object
        static VoteRequest fromJson(QJsonObject json);
    };

    /// @brief vote response after receiving vote request
    struct VoteResponse {
        qint64 term;            ///< @brief responder's term number
        qint32 id;              ///< @brief responder's id
        bool approve;           ///< @brief is the voterequest approved

        /// @brief convert to json
        /// @return the converted json object
        QJsonObject toJson();

        /// @brief convert from json
        /// @param json the json object to convert
        /// @return the constructed VoteResponse object
        static VoteResponse fromJson(QJsonObject json);
    };

    /// @brief append entry request
    struct AppendEntry {
        qint32 id;              ///< @brief sender's id
        qint64 term;            ///< @brief sender's term

        /// @brief convert to json
        /// @return the converted json object
        QJsonObject toJson();

        /// @brief convert from json
        /// @param json the json object to convert
        /// @return the constructed VoteResponse object
        static AppendEntry fromJson(QJsonObject json);
    };

    /// @brief nodes data (used for all known nodes)
    struct Node {
        qint32 id;              ///< @brief node's id
        bool votedForMe;        ///< @brief is this node voted for me
    };
    QMap<quint32, Node>allNodes;///< @brief all known nodes (including myself), map id to Node

    qint32 id;                  ///< @brief this node's id
    qint32 currentLeader;       ///< @brief who this node think is the current leader
    qint64 currentTerm;         ///< @brief the current term number of this node
    qint32 votedTo;             ///< @brief to which machine I have voted in this term
    State state;                ///< @brief this node's state: follower/candidate/leader

    /// @brief the base timeout of election (real timeout will be in (50%~150%) bET)
    static const quint32 baseElectionTimeout;
    /// @brief the base timeout of appendentry (real timeout will be in (50%~150%) bET)
    static const quint32 baseAppendEntryTimeout;
    quint32 appendEntryTimeout; ///< @brief the timeout interval for syncing
    quint32 electionTimeOut;    ///< @brief the timeout interval for next election
    QTimer timer;               ///< @brief do routines periodically
    quint64 elapsedTime;        ///< @brief elapsed time since last reset, added periodically by timer

    bool started;               ///< @brief is the simulation of this node started

    /// @brief Randomize the electionTimeout and reset elapsedTime
    void resetTimeout();

    /// @brief Called when start a election. \n
    ///
    /// this will cause this node to become a candidate
    void startElection();

    /// @brief vote for a node
    /// @param id the id of the node to vote for
    /// @param approve is the vote request approved
    void voteFor(qint32 id, bool approve);

    /// @brief request vote from a node
    /// @param id id of the node to request vote from
    void requestVoteFrom(qint32 id);

    /// @brief broadcast append entry to all nodes
    void broadcastAppendEntry();

    /// @brief send info outside
    /// @param infoList the list of info to send
    ///
    /// this method will emit info() signal
    void sendInfo(QList<QPair<InfoKey, QJsonValue>> infoList);

    /// @brief judge if this node vote should vote for a vote request
    /// @param request the vote request to judge
    bool shouldVote(VoteRequest request);

    /// @brief set term and reset necessary values for the new term
    void setCurrentTerm(qint64 newTerm);
private slots:
    /// @brief routines that are executed periodically in this function
    void routine();

public:
    explicit RaftNode(QObject *parent = nullptr);

    /// @brief start the timer, activate this node
    /// @param allNodesId all nodes' id including this node, each one is unique globally
    /// @param myId this node's id
    /// @param speed simulation speed (inverval for each timeout, in msec)
    /// @note this method should be only called once. subsequence calles are invalid.
    void run(QList<qint32> allNodesId, qint32 myId, qint32 speed);

    /// @brief get this node's id
    /// @return this node's id
    qint32 getId();

    /// @brief set the timer's speed
    /// @param speed the speed to set
    void setTimerSpeed(qint32 speed);

    /// @brief get this node's state
    /// @return this node's state
    State getState() const;

signals:

    /// @brief emitted when anything that might require a attention happened
    /// @param info a json format information object\n
    /// @see InfoKey
    void info(QJsonObject info);

    /// @brief emitted when this node trys to send vote request to other nodes
    /// @param id the node to send vote request to
    /// @param voteRequest the vote request to send
    void sendVoteRequestTo(qint32 id, QJsonObject voteRequest);

    /// @brief emitted when this node trys to send vote to other nodes
    /// @param id the node to send vote to
    /// @param voteResponse the vote response to send
    void sendVoteTo(qint32 id, QJsonObject voteResponse);

    /// @brief send append entry request
    /// @param id the node to send vote request to
    /// @param appendEntry the append entry to send
    void sendAppendEntryTo(qint32 id, QJsonObject appendEntry);
public slots:
    /// @brief shold be triggered when a vote request arrives
    /// @param voteRequest the vote request from other node
    void receiveVoteRequest(QJsonObject voteRequest);

    /// @brief should be triggered when a vote response arrives
    /// @param voteResponse the response coming form other nodes
    void receiveVoteResponse(QJsonObject voteResponse);

    /// @brief should be triggered when a appendentry arrives
    /// @param appendEntry the append entry object from leader
    void receiveAppendEntry(QJsonObject appendEntry);
};

#endif // RAFTNODE_H
