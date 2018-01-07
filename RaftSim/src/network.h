/// @file network.h
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#ifndef NETWORK_H
#define NETWORK_H

#include "raftnode.h"

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QQueue>
#include <QJsonObject>

/// @brief this class simulates the network that handles all message
/// transfers between RaftNodes;
///
/// In real world, the data transmit part of this class
/// is substituted with a real network environment,
/// with all RaftNodes instance run on different servers.
/// (this is only a simple and crude simulator)
class Network : public QObject {
    Q_OBJECT
private:
    /// @brief message type
    enum MessageType {
        typeVoteRequest,                    ///< @brief vote request
        typeVoteRespond,                    ///< @brief vote respond
        typeAppendEntry,                    ///< @brief append entry
    };

    /// @brief message struct
    struct Message {
        QTimer *timer;                      ///< @brief singleshot timer, send message when timeout
        qint32 dstId;                       ///< @brief destination node id
        MessageType type;                   ///< @brief type of this messgage
        QJsonObject data;                   ///< @brief data to send
    };

    QMap<qint32, RaftNode *> allNodes;      ///< @brief all nodes known by this network
    QMap<qint32, bool> nodesStatus;         ///< @brief a map indicating that is the node functional
    qint32 propagationTime;                 ///< @brief propagation time simulation (in msec)
    QMap<QTimer *, Message> messages;       ///< @brief messages pending to send
    bool propagationTimeRandomized;         ///< @brief is propagatino time randomized

    /// @brief create a message struct and add it to messages
    /// @brief data data to send
    /// @brief type type of the message
    /// @brief dstId destination node id
    void addToMessageList(QJsonObject data, MessageType type, qint32 dstId);
public:
    explicit Network(QObject *parent = nullptr);

    /// @brief start the network simulation
    /// @param allNodes all the nodes known, a map of {id, RaftNode-object-pointer}
    /// @note this method should only be called once. subsequence calls are invalid
    void run(QMap<qint32, RaftNode *> allNodes);

    /// @brief set the propagation time
    /// @param time the propagation interval (in msec)
    void setPropagationTime(qint32 time);

    /// @brief disconnect one node from internet to simulate a physical fault
    /// @param nodeid id of the node to takedown
    void takeNodeDown(qint32 nodeid);

    /// @brief re-connect one node from internet to simulate node recovery
    /// @param nodeid the id of the node to bring up
    void bringNodeUp(qint32 nodeid);

signals:
    /// @brief simulate the process that data goes out of network and reach the target
    /// @param data the data coming out the network
    /// @note this signal sould be ignored outside the module
    void dataOut(QJsonObject data);

private slots:
    /// @brief time to send a message!
    void sendMessageTimedOut();

    /// @brief this slot should me triggerd when a voteRequest goes into network
    /// @param dstId destination node id
    /// @param data data to send
    void voteRequestIn(qint32 dstId, QJsonObject data);

    /// @brief this slot should me triggerd when a voteRespond goes into network
    /// @param dstId destination node id
    /// @param data data to send
    void voteRespondIn(qint32 dstId, QJsonObject data);

    /// @brief this slot should me triggerd when a appendEntry goes into network
    /// @param dstId destination node id
    /// @param data data to send
    void appendEntryIn(qint32 dstId, QJsonObject data);

public slots:
    /// @brief set if the propagation time should be random
    /// @param state is the propagation time randomized
    void setRandomizePropagationTime(bool state);

};

#endif // NETWORK_H
