/// @file mainwindow.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::showInfo(QString text) {
    ui->labelStatus->setText(text);
}

void MainWindow::showWarning(QString text) {
    //> show a label and shake it to attract some attention
    ui->labelStatus->setText(text);
    shakeAnimation.start();
}

void MainWindow::initTestCase(QString testCase, QString inputType, QSharedPointer<bool> uiLocker) {
    //> init ui
    QTreeWidgetItem *inputLine = new QTreeWidgetItem({tr("input/") + inputType, testCase, tr("pending")});
    QTreeWidgetItem *stdOutputLine = new QTreeWidgetItem({tr("output/standard")});
    QTreeWidgetItem *userOutputLine = new QTreeWidgetItem({tr("output/yours")});
    QTreeWidgetItem *spacer = new QTreeWidgetItem({tr("")});

    ui->treeTestResult->addTopLevelItems({inputLine, stdOutputLine, userOutputLine, spacer});

    //> init test case
    TestCaseInfo info;
    info.uiLocker = uiLocker;
    info.input = testCase;
    info.phase = TestCaseInfo::phaseStandard;
    info.inputType = inputType;
    info.inputLine = inputLine;
    info.standardOutputLine = stdOutputLine;
    info.userOutputLine = userOutputLine;
    info.process = QSharedPointer<QProcess>(new QProcess);
    info.timer = QSharedPointer<QTimer>(new QTimer);

    connect(info.process.data(), SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(testCaseFinished(int, QProcess::ExitStatus)));
    connect(info.process.data(), SIGNAL(errorOccurred(QProcess::ProcessError)),
            this, SLOT(testCaseErrorOccurred(QProcess::ProcessError)));
    connect(info.timer.data(), SIGNAL(timeout()),
            this, SLOT(testCaseTimedOut()));

    //> add test case to quque
    pendingCases.insert(info.process.data(), info);
    pendingCasesTimerMap.insert(info.timer.data(), info.process.data());
}

bool MainWindow::checkExecutablesValidity() {
    using namespace Resources;
    QAbstractButton *checked = ui->buttonGroupStandardExecs->checkedButton();
    QDir standardExecDir(QApplication::applicationDirPath());

    //> check standard executable dir's existence
    if (! standardExecDir.cd(resStandardExecDirPath)) {
        showWarning(tr("Folder \"")
                + resStandardExecDirPath
                + tr("/\" not found. Please make sure it exists."));
        return false;
    }

    //> get the standard executable
    QString standardExecFileName;
    if (checked == ui->buttonStandardStack) {
        standardExecFileName = resStandardStackExecName;
    } else if (checked == ui->buttonStandardQueue) {
        standardExecFileName = resStandardQueueExecName;
    } else if (checked == ui->buttonStandardQueueByStack) {
        standardExecFileName = resStandardQueueByStackExecName;
    } else {
        showWarning(tr("Please <span style=\"font-weight:600;\">select</span> a standard executable first."));
        return false;
    }

    //> check standard executable's validity
    //>> check standard executable's existence
    if (!standardExecDir.entryList(QDir::Files).contains(standardExecFileName)) {
        showWarning(tr("Standard executable \"") + standardExecFileName + tr("\" does not exist."));
        return false;
    }
    standardExecFilePath = standardExecDir.filePath(standardExecFileName);

    //>> check standard executable's validity
    if (!QFileInfo(standardExecFilePath).isExecutable()) {
        showWarning(tr("Standard executable \"") + standardExecFileName + tr("\" is not an executable."));
        return false;
    }
    //>> check if user executable exists
    if (userExecFilePath.isEmpty()) {
        showWarning(tr("Please <span style=\"font-weight:600;\">select</span> your executable."));
        return false;
    }

    //> check user executable's validity
    QFileInfo userExecInfo(userExecFilePath);
    //>> check if file exist
    if (!userExecInfo.exists()) {
        showWarning(tr("Your executable does not exist."));
        return false;
    }
    //>> check if it's a executable file
    if (!userExecInfo.isExecutable()) {
        showWarning(tr("Your executable is not an <span style=\"font-weight:600;\">executable</span> file."));
        return false;
    }

    //> check passed
    return true;
}

void MainWindow::loadUserExecutable(QString filePath) {
    QFileInfo fileInfo(filePath);

    //> check if file exist
    if (!fileInfo.exists()) {
        showWarning(tr("User File does not exist."));
        return;
    }

    //> check if it's a file
    if (!fileInfo.isFile()) {
        showWarning(tr("This is not a <span style=\"font-weight:600;\">file</span>."));
        return;
    }

    //> check if it's a executable file
    if (!fileInfo.isExecutable()) {
        showWarning(tr("This is not an <span style=\"font-weight:600;\">executable</span> file."));
        return;
    }

    //> check passed, set the file
    userExecFilePath = filePath;
    ui->browserFileName->setText(filePath);
}

void MainWindow::loadCustomTestFile(QString filePath) {
    QFileInfo fileInfo(filePath);

    //> check if file exist
    if (!fileInfo.exists()) {
        showWarning(tr("User File does not exist."));
        return;
    }

    //> check if it's a file
    if (!fileInfo.isFile()) {
        showWarning(tr("This is not a <span style=\"font-weight:600;\">file</span>."));
        return;
    }

    //> check file size
    quint64 size = fileInfo.size();
    quint64 maxSize = Resources::resMaxUserTestCaseFileSize;
    if (size > maxSize) {
        QString metric("B");
        if (maxSize >= 1000) {
            maxSize /= 1000;
            metric = QString("kB");
        }
        if (maxSize >= 1000) {
            maxSize /= 1000;
            metric = QString("MB");
        }
        if (maxSize >= 1000) {
            maxSize /= 1000;
            metric = QString("GB");
        }
        showWarning(tr("File exceeds ") + QString::number(maxSize) + metric + QString("."));
        return;
    }

    //> load data from file
    QFile testCaseFile(filePath) ;
    if (!testCaseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showWarning(tr("Failed to <span style=\"font-weight:600;\">open</span> this file."));
        return;
    };
    QList<QByteArray> rawTestCases = testCaseFile.readAll().split('\n');
    testCaseFile.close();

    //> validate and load to custom Test Case
    customTestCases.clear();
    quint64 count = 0;
    for (QByteArray rawTestCase : rawTestCases) {
        QString testCase = getTestCaseFromData(QString(rawTestCase));
        if (testCase.size()) {
            customTestCases.append(testCase);
            ++count;
        }
    }

    //> setup ui
    ui->browserTestFileName->setText(
            QString("<span style=\"font-weight:600;\">")
            + QString::number(count) + tr(" case(s) loaded\n")
            + QString("</span>")
            + filePath);
    showInfo(QString::number(count) + tr(" case(s) loaded."));
}

void MainWindow::startTestChain() {
    //> start running all cases
    if (pendingCases.empty())
        return;

    //> start timer
    pendingCases.first().timer->start(Resources::resTestCaseTimeOutInterval);
    pendingCases.first().process->start(
            pendingCases.first().phase == TestCaseInfo::phaseStandard ? standardExecFilePath : userExecFilePath,
            pendingCases.first().input.replace('\t', ' ').split(' ', QString::SkipEmptyParts));
}

void MainWindow::updateTestProgress() {
    //> set progress
    showInfo(QString::number(totalTestCaseNum - pendingCases.size())
            + QString("/") + QString::number(totalTestCaseNum) + tr(" finished"));
    ui->progressBar->setValue((totalTestCaseNum - pendingCases.size()) * 100 / totalTestCaseNum);
}

QString MainWindow::getTestCaseFromData(QString rawData) {
    //> cannot be empty after removing comments
    if (rawData.trimmed().left(rawData.trimmed().lastIndexOf('#')).isEmpty())
        return QString();

    //> validate line
    QRegExp validation("((^|\\s+)-[SIOCAGN](\\s+\\d+\\s*)*)+(#.*)?");

    if (!validation.exactMatch(rawData))
        return QString();

    //> remove comments, reform it, return
    return rawData
            .left(rawData.lastIndexOf('#'))
            .replace(QRegExp("\\s"), " ")
            .split(' ').join("  ");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //> connect ui signals
    //>> start test signal
    connect(ui->buttonStartTest, SIGNAL(clicked()), this, SLOT(buttonStartTestClicked()));
    //>> start single test signal
    connect(ui->buttonExecute, SIGNAL(clicked()), this, SLOT(buttonSingleTestClicked()));
    //>> load executable signal
    connect(ui->buttonLoadExec, SIGNAL(clicked()), this, SLOT(buttonLoadExecClicked()));
    //>> drag'n'drop executable signal
    connect(ui->browserFileName, SIGNAL(fileDropped(QList<QUrl>)), this, SLOT(fileDropped(QList<QUrl>)));
    //>> load test case file signal
    connect(ui->buttonLoadCustomCase, SIGNAL(clicked()), this, SLOT(buttonLoadCustomTestCaseClicked()));
    //>> drag'n'drop test case file signal
    connect(ui->browserTestFileName, SIGNAL(fileDropped(QList<QUrl>)), this, SLOT(testCaseFileDropped(QList<QUrl>)));

    //> menu items
    //>> about dialog
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actionAboutTriggered()));
    //>> language changing
    connect(ui->actionLanguage, &QAction::triggered, [ = ]() {
        if (!Global::globalCurrentTranslator) {
            Global::globalCurrentTranslator = new QTranslator();
            Global::globalCurrentTranslator->load(":/translations/CheckSystemPlusPlus_zh-CN.qm");
            qApp->installTranslator(Global::globalCurrentTranslator);
            ui->retranslateUi(this);
        } else {
            qApp->removeTranslator(Global::globalCurrentTranslator);
            delete Global::globalCurrentTranslator;
            Global::globalCurrentTranslator = nullptr;
            ui->retranslateUi(this);
        }
    });

    //> init ui
    ui->progressBar->hide();

    //> init some members
    QEasingCurve shakeCurve(QEasingCurve::OutElastic);
    shakeCurve.setAmplitude(3.5);
    shakeCurve.setPeriod(0.25);
    shakeAnimation.setTargetObject(ui->widgetShake);
    shakeAnimation.setPropertyName("maximumSize");
    shakeAnimation.setEasingCurve(shakeCurve);
    shakeAnimation.setStartValue(QSize(0, 10));
    shakeAnimation.setEndValue(QSize(20, 10));
    shakeAnimation.setDuration(500);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::buttonStartTestClicked() {
    using namespace Resources;
    //> disable all input while running
    ui->widgetInput->setEnabled(false);
    ui->editTempCase->setEnabled(false);
    ui->buttonExecute->setEnabled(false);
    ui->progressBar->show();
    //> re-enable all input after running
    QSharedPointer<bool> uiLocker(new bool, [ = ](bool *) {
        ui->widgetInput->setEnabled(true);
        ui->editTempCase->setEnabled(true);
        ui->buttonExecute->setEnabled(true);
        ui->progressBar->hide();
    });

    //> check the validity of standard and user's executables
    if (!checkExecutablesValidity())
        return;

    //> clear ui (must be before the initialize)
    ui->treeTestResult->clear();
    ui->labelStatus->clear();

    //> set total test case number
    totalTestCaseNum = resDefaultTestCases.size() + customTestCases.size();

    //> init standard test case
    for (QString testCase : resDefaultTestCases)
        initTestCase(testCase, tr("default"), uiLocker);

    //> init user test case
    for (QString testCase : customTestCases)
        initTestCase(testCase, tr("custom"), uiLocker);

    startTestChain();
}

void MainWindow::buttonLoadExecClicked() {
    QString filePath = QFileDialog::getOpenFileName(
                    this, tr("Choose your executable"), "",
                    tr("Executables (*.exe);;"));

    //> check validity, try set the file
    loadUserExecutable(filePath);
}

void MainWindow::buttonLoadCustomTestCaseClicked() {

    QString filePath = QFileDialog::getOpenFileName(
                    this, tr("Choose your executable"), "",
                    tr("Text (*.txt *.TXT);;"));

    //> check validity, try set the file
    loadCustomTestFile(filePath);
}

void MainWindow::fileDropped(QList<QUrl> filePaths) {
    //> clear status
    ui->labelStatus->clear();

    //> validate file
    if (filePaths.size() != 1) {
        //> if dragged more than one file
        showWarning(tr("Only <span style=\"font-weight:600;\">one</span> file at a time."));
        return;
    }

    QString filePath = filePaths.first().toLocalFile();
    loadUserExecutable(filePath);
}

void MainWindow::testCaseFileDropped(QList<QUrl> filePaths) {
    //> clear status
    ui->labelStatus->clear();

    //> validate file
    if (filePaths.size() != 1) {
        //> if dragged more than one file
        showWarning(tr("Only <span style=\"font-weight:600;\">one</span> file at a time."));
        return;
    }

    QString filePath = filePaths.first().toLocalFile();
    loadCustomTestFile(filePath);
}

void MainWindow::testCaseFinished(int, QProcess::ExitStatus status) {
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (!pendingCases.contains(process))
        return;

    TestCaseInfo &info = pendingCases[process];
    //> judge the process phase (standard or user's)
    if (info.phase == TestCaseInfo::phaseStandard) {
        if (status == QProcess::CrashExit) {
            info.standardOutputLine->setText(1, tr("crashed"));
        } else {
            QString output = process->readAll();
            //> get output and setup ui
            info.standardOutputLine->setText(1,
                    output.replace(QRegExp("\\s"), " ").split(' ', QString::SkipEmptyParts).join("  "));
        }

        //> change to next phase and execute next phase
        info.phase = TestCaseInfo::phaseUser;
        startTestChain();
    } else {
        if (status == QProcess::CrashExit) {
            info.userOutputLine->setText(1, tr("crashed"));
        } else {
            //> try to get the file
            QFileInfo execInfo(userExecFilePath);
            QString baseName(execInfo.fileName()
                    .left(execInfo.fileName().lastIndexOf(".exe")));
            QFile resultFile(baseName + ".TXT");
            if (!resultFile.exists())
                resultFile.setFileName(baseName + ".txt");

            QString output;
            //> first try get output from file, then from stdout
            if (resultFile.exists()) {
                resultFile.open(QIODevice::ReadOnly | QIODevice::Text);
                output =  resultFile.readAll();
                output = output.left(output.lastIndexOf('\n'));
                resultFile.close();

                //> don't forget to clear the buffer
                process->readAll();
            } else {
                //> get output and setup ui
                output = process->readAll();
            }

            info.userOutputLine->setText(1,
                    output.replace(QRegExp("\\s"), " ").split(' ', QString::SkipEmptyParts).join("  "));
        }

        //> set correctness color
        QColor bgColor;
        if (info.userOutputLine->text(1) == info.standardOutputLine->text(1)) {
            info.inputLine->setText(2, tr("accepted"));
            bgColor = Resources::resAcceptedColor;
        } else {
            info.inputLine->setText(2, tr("wrong answer"));
            bgColor = Resources::resWrongAnswerColor;
        }
        //> set color
        for (int i = 0; i < info.inputLine->columnCount(); ++i)
            info.inputLine->setBackgroundColor(i, bgColor);

        //> this case is finished, remove from map
        pendingCasesTimerMap.remove(info.timer.data());
        pendingCases.remove(process);

        //> run next case if necessary
        if (!pendingCases.isEmpty()) {
            //> run next case
            startTestChain();
        } else {
            //> all finished,auto-resize UI
            int count = ui->treeTestResult->topLevelItemCount();
            for (int i = 0; i < count; ++i)
                ui->treeTestResult->resizeColumnToContents(i);
            ui->treeTestResult->scrollToBottom();
        }

        //> update progress
        updateTestProgress();
    }
}

void MainWindow::testCaseErrorOccurred(QProcess::ProcessError error) {
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (!pendingCases.contains(process))
        return;

    //> only deal with failedToStart error
    if (error != QProcess::FailedToStart)
        return;

    TestCaseInfo &info = pendingCases[process];
    //> judge the process phase (standard or user's)
    if (info.phase == TestCaseInfo::phaseStandard) {
        info.standardOutputLine->setText(1, tr("failed to start (executable missing or permission problem)"));

        //> change to next phase and execute next phase
        info.phase = TestCaseInfo::phaseUser;
        info.process->start(userExecFilePath,
                info.input.replace('\t', ' ').split(' ', QString::SkipEmptyParts));
    } else {
        //> setup UI
        //>> set text
        info.userOutputLine->setText(1, tr("failed to start (executable missing or permission problem)"));
        //>> set color
        for (int i = 0; i < info.inputLine->columnCount(); ++i)
            info.inputLine->setBackgroundColor(i, Resources::resFailedToStartColor);

        //> set progress
        showInfo(QString::number(totalTestCaseNum - pendingCases.size())
                + QString("/") + totalTestCaseNum + tr("finished"));
        ui->progressBar->setValue((totalTestCaseNum - pendingCases.size()) * 100 / totalTestCaseNum);

        //> this case is finished, remove from map
        pendingCases.remove(process);

        //> run next case if necessary
        if (!pendingCases.isEmpty()) {
            //> run next case
            startTestChain();
        } else {
            //> all finished,auto-resize UI
            int count = ui->treeTestResult->topLevelItemCount();
            for (int i = 0; i < count; ++i)
                ui->treeTestResult->resizeColumnToContents(i);
            ui->treeTestResult->scrollToBottom();
        }

        //> update progress
        updateTestProgress();
    }
}

void MainWindow::testCaseTimedOut() {
    QTimer *timer = qobject_cast<QTimer *>(sender());
    if (pendingCasesTimerMap.isEmpty())
        return;

    //> get the info
    QProcess *process = pendingCasesTimerMap[timer];
    TestCaseInfo &info = pendingCases[process];

    //> stop the program
    info.process->disconnect();
    info.process->kill();

    //> release resources
    pendingCasesTimerMap.remove(timer);
    pendingCases.remove(process);

    //> UI
    //>> set text
    info.inputLine->setText(2, tr("timed out"));
    //>> set color
    for (int i = 0; i < info.inputLine->columnCount(); ++i)
        info.inputLine->setBackgroundColor(i, Resources::resTimedOutColor);
    //>> set progres
    updateTestProgress();

    //> do rest tests
    startTestChain();
}

void MainWindow::buttonSingleTestClicked() {
    //> disable all input while running
    ui->widgetInput->setEnabled(false);
    ui->editTempCase->setEnabled(false);
    ui->buttonExecute->setEnabled(false);
    //> re-enable all input after running
    QSharedPointer<bool> uiLocker(new bool, [ = ](bool *) {
        ui->widgetInput->setEnabled(true);
        ui->editTempCase->setEnabled(true);
        ui->buttonExecute->setEnabled(true);
    });

    //> make sure input is not empty
    if (ui->editTempCase->text().isEmpty()) {
        showWarning("Input is <span style=\"font-weight:600;\">empty.</span>");
        return;
    }

    QString testCase = getTestCaseFromData(ui->editTempCase->text());
    if (!testCase.size()) {
        showWarning("Input <span style=\"font-weight:600;\">invalid</span>.");
        return;
    }

    //> check standard/userexecutable validity
    if (!checkExecutablesValidity())
        return;

    //> clear ui
    ui->labelStatus->clear();

    //> add test
    initTestCase(testCase, tr("singleShot"), uiLocker);

    //> start chain
    startTestChain();
}

void MainWindow::actionAboutTriggered() {
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->show();
}
