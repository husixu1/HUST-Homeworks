/// @file servercore.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef SERVERCORE_H
#define SERVERCORE_H

#include "baseconnector.h"
#include "dataprocessor.h"

#include <QObject>
#include <QHostAddress>
#include <QThread>
#include <QSharedPointer>

class ServerCore : public QObject {
    Q_OBJECT
private:
    static const quint16 serverPort = 5100;
    /// @brief constructor
    ServerCore() = default;

    /// @brief only one connector per server
    BaseConnector *connector;
    /// @brief the thread that this connector is running on
    QThread connectorThread;

    /// @brief only one data processor per server
    DataProcessor *processor;
    /// @brief the thread that this processor is runnign on
    QThread processorThread;

public:

    /// @brief get the singleton instance
    static ServerCore &getInstance();

    /// @brief initialize BaseConnector and DataProcessor in other two threads
    void init();

    /// @brief singleton assurance
    ServerCore(const ServerCore &) = delete;
    /// @brief singleton assurance
    ServerCore &operator =(const ServerCore &) = delete;
    /// @brief de-constructor
    ~ServerCore();

    /// @brief get the inited connector
    const BaseConnector *getConnector();

signals:
    /// @brief signal sent to baseConnector, do not use
    startListen(quint16 port);

private slots:
    void listenFailed();

    void listenStarted(QHostAddress address, quint16 port);
public slots:
};

#endif // SERVERCORE_H
