#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "dbconnector.h"
#include "httpserver.h"

#include <QObject>
#include <QThread>

/// @brief manages start, stop and schedule of server services
class ServiceManager : public QObject {
    Q_OBJECT
private:
    /// @brief the thread running database connector //TODO: multiple thread
    QThread *dbThread;
    /// @brief database connectors
    DBConnector *dbConnector;

    /// @brief the http server //TODO: make it run in another thread
    HttpServer *httpServer;

    /// @brief singleton constructor
    explicit ServiceManager(QObject *parent = nullptr);

public:
    static ServiceManager &getInstance();

    /// @brief initialize all services
    void initialize();

    /// @brief stop services and do clenup jobs
    /// @note it's not safe to call initialize() again after calling
    /// this function and before serviceStopped() signal is emitted.
    /// The program will deal with remaining sql requests during this interval.
    void stopAndClenup();

    // singleton specifics
    ServiceManager(const ServiceManager &) = delete;
    ServiceManager &operator =(const ServiceManager &) = delete;

    /// @brief de-constructor
    ~ServiceManager();

    HttpServer *getHttpServer() const;
signals:

    /// @brief emitted when all services are stopped
    /// it's not safe to start service again before this signal is emitted
    void serviceStopped();
public slots:
};

#endif // SERVICEMANAGER_H
