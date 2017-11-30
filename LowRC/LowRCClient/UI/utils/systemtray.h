#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include<QSystemTrayIcon>
#include<QObject>

class SystemTray : public QSystemTrayIcon {
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);
    void setTrayIcon(QString iconPath);
};

#endif // SYSTEMTRAY_H
