/// @file mainwindow.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutdialog.h"

#include "omnitextbrowser.h"
#include "res.h"

#include <QMainWindow>
#include <QString>
#include <QtDebug>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QList>
#include <QUrl>
#include <QTextBrowser>
#include <QStringList>
#include <QColor>
#include <QMap>
#include <QQueue>
#include <QTreeWidgetItem>
#include <QSharedPointer>
#include <QRegExp>
#include <QTimer>
#include <QFileDialog>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    struct TestCaseInfo {
        QSharedPointer<QProcess> process;
        QSharedPointer<bool> uiLocker;
        QSharedPointer<QTimer> timer;
        enum {phaseStandard, phaseUser} phase;
        QString inputType;
        QString input;
        quint64 inputIndex;
        QTreeWidgetItem *inputLine;
        QTreeWidgetItem *standardOutputLine;
        QTreeWidgetItem *userOutputLine;
    };

    QString standardExecFilePath;                       ///< @brief the standard execution
    QString userExecFilePath;                           ///< @brief the user's executive file name

    QStringList customTestCases;                        ///< @brief the user defined test cases
    QMap<QProcess *, TestCaseInfo> pendingCases;        ///< @brief the pending test cases to run
    QMap<QTimer *, QProcess *> pendingCasesTimerMap;    ///< @brief deal with timeout event
    quint64 totalTestCaseNum;

    QPropertyAnimation shakeAnimation;                  ///< @brief the animation used by the statusLabel


    /// @brief show a status that does not need much attention
    void showInfo(QString text);

    /// @brief show a status that needs attention
    void showWarning(QString text);

    /// @brief initialize a new test case (with ui initialize)
    void initTestCase(QString testCase, QString inputType, QSharedPointer<bool> uiLocker);

    /// @brief check standard executable and user's executable's validity
    /// this function should only be used before executing test cases
    bool checkExecutablesValidity();
    /// @brief check user executable's validity
    /// this function should only be used when trying to load executable
    void loadUserExecutable(QString filePath);
    /// @brief try set custom test file
    /// this function should only be used when trying to load custom test case
    void loadCustomTestFile(QString filePath);

    /// @brief try start the test case testing chain
    /// caller should gurantee executable is valid using checkExecutablesValidity()
    inline void startTestChain();

    /// @brief set the execution progress
    inline void updateTestProgress();

    /// @brief get test case from a raw stirng
    /// @return the test case, or empty for invalid
    QString getTestCaseFromData(QString rawData);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /// @brief when the start-test button is clicked
    void buttonStartTestClicked();
    /// @brief when load exec is clicked
    void buttonLoadExecClicked();
    /// @brief when load custom test case button is clicked
    void buttonLoadCustomTestCaseClicked();

    /// @brief when file dragged in, try set user's executable
    void fileDropped(QList<QUrl> filePaths);
    /// @brief the test case is dropped in, try set the user's test case
    void testCaseFileDropped(QList<QUrl> filePaths);

    /// @brief when a test case process is finished
    void testCaseFinished(int returnVal, QProcess::ExitStatus status);
    /// @brief when a test case process suffers an error
    void testCaseErrorOccurred(QProcess::ProcessError error);
    /// @brief when a test case timed out
    void testCaseTimedOut();

    /// @brief the buttonExecute clicked, perform a single test
    void buttonSingleTestClicked();

    /// @brief the about menu item is triggered
    void actionAboutTriggered();
};

#endif // MAINWINDOW_H
