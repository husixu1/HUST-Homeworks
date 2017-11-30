#include"systemtray.h"
#include"res.h"

SystemTray::SystemTray(QObject *parent) : QSystemTrayIcon(parent) {

}

void SystemTray::setTrayIcon(QString iconPath) {
    //this->setIcon(QIcon(res_sysTrayIcon));
    this->setIcon(QIcon(iconPath));
}
