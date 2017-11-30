/// @file mainwindow.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include "servercore.h"
#include "globalconfig.h"
#include "passworddialog.h"
#include "changepassworddialog.h"

#include <QMainWindow>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QCloseEvent>
#include <QMessageBox>
#include <QCryptographicHash>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /// @brief the updating time, and update time factor
    /// the updating time is desinged to have a liner relationship with the database size,
    /// that is, the larger the databse, the slower the view refreshes. updateTime = 100 + updateFactor * databaseSize
    /// @see updateFactor
    int updateTime;
    /// @brief the updateTime factor
    /// @see updateTime
    static const int updateFactor = 2;

    /// @brief update minimal interval, to forbid a too-frequent update
    QTimer *updateTimer;
    /// @brief is the view needed to be updated now
    bool needUpdate;

    /// @brief do something before close the window
    void closeEvent(QCloseEvent *event) override;

    /// @brief used by detail panel
    QString currentUserEmail;

    /// @brief used by detail panel
    void updateDetailPanelInfo(QString userEmail);

    /// @brief load the config file loaded successfully
    bool isConfigLoaded;

private slots:
    /// @brief update the user's list
    void updateUserList();

    /// @brief triggered by the update Timer, indicates that can perform a update
    void updateTimedOut();

    /// @brief the user line is clicked, show details
    void userClicked(QModelIndex index);

    /// @brief validate passwd and load database
    void validateButtonClicked();
    /// @brief start validating passwd
    void validatePasswd(QString);

    /// @brief chane the passwd
    void changePasswdButtonClicked();
    /// @brief try changing the passwd
    void changePasswd(QString, QString);
};

#endif // MAINWINDOW_H
