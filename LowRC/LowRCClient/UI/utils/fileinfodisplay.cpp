/// @file fileinfodisplay.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "fileinfodisplay.h"
#include "ui_fileinfodisplay.h"

void FileInfoDisplay::setCorrespondingOperation(QSharedPointer<OperationDescriptor> value) {
    correspondingOperation = value;
}

void FileInfoDisplay::setTotalBytes(const quint64 &value) {
    totalBytes = value;
}

void FileInfoDisplay::speedTimerTimedOut() {
    //> just add timeout count every turn
    ++timeoutCount;
    canUpdate = true;
}

FileInfoDisplay::FileInfoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileInfoDisplay) {
    ui->setupUi(this);

    connect(&speedTimer, SIGNAL(timeout()), this, SLOT(speedTimerTimedOut()));

    totalBytes = 0;
    timeoutCount = 0;
    canUpdate = true;
}

FileInfoDisplay::~FileInfoDisplay() {
    delete ui;
}

void FileInfoDisplay::setStatus(FileInfoDisplay::Status status) {
    currenStatus = status;

    for (QWidget *widget : omniLayoutWidgets)
        delete widget;
    omniLayoutWidgets.clear();

    switch (status) {
        case statusSendInitial: {
            QLabel *info = new QLabel;
            omniLayoutWidgets.append(info);
            //> add to layout
            info->setText("waiting for accept");
            ui->omniLayout->addWidget(info);
            break;
        }
        case statusSendBegan: {
            speedTimer.start(100);
            speedLabel = new QLabel;
            omniLayoutWidgets.append(speedLabel);
            //> add to layout
            ui->omniLayout->addWidget(speedLabel);
            break;
        }
        case statusReceiveInitial: {
            ClickableLabel *acceptLabel = new ClickableLabel(this);
            ClickableLabel *declineLabel = new ClickableLabel(this);
            omniLayoutWidgets.append(acceptLabel);
            omniLayoutWidgets.append(declineLabel);
            //> connect signals
            connect(acceptLabel, &ClickableLabel::clicked, [ = ]() {
                emit fileAccecpted(correspondingOperation);
            });
            connect(declineLabel, &ClickableLabel::clicked, [ = ]() {
                emit fileDeclined(correspondingOperation);
            });

            //> add label to layout
            acceptLabel->setText("accept");
            declineLabel->setText("decline");
            ui->omniLayout->addWidget(acceptLabel);
            ui->omniLayout->addWidget(declineLabel);
            break;
        }
        case statusReceiveBegan: {
            speedTimer.start(100);
            speedLabel = new QLabel;
            omniLayoutWidgets.append(speedLabel);
            //> add to layout
            ui->omniLayout->addWidget(speedLabel);
            break;
        }
        case statusReceiveFinished:
        case statusSenFinished: {
            speedTimer.stop();
            QLabel *finishLabel = new QLabel;
            omniLayoutWidgets.append(finishLabel);
            ui->omniLayout->addWidget(finishLabel);
            delete ui->progressBar;

            //> conver speed
            //>> timeout every 0.1 sec, so bytes/(count/10) == (bytes*10)/count yeah
            double bytesPerSec = static_cast<double>(totalBytes) * 10.0 / static_cast<double>(timeoutCount);
            QString metric = "Bytes/sec";
            if (bytesPerSec > 1500) {
                bytesPerSec /= 1000.0;
                metric = "KBytes/sec";
            }
            if (bytesPerSec > 1500) {
                bytesPerSec /= 1000.0;
                metric = "MBytes/sec";
            }
            //> set text
            finishLabel->setText("average " + QString::number(bytesPerSec) + metric);
            break;
        }
        default:
            break;
    }
}

void FileInfoDisplay::setFileName(QString filename) {
    ui->fileName->setText(filename);
}

void FileInfoDisplay::setFileIcon(QIcon fileIcon) {
    //TODO: set file icon
}

void FileInfoDisplay::setProgress(quint64 bytesProceeded) {
    //> set the progressbar progress
    if (canUpdate) {
        if (totalBytes)
            ui->progressBar->setValue(static_cast<int>(bytesProceeded * 100 / totalBytes));

        //> timeout every 0.1 sec, so bytes/(count/10) == (bytes*10)/count yeah
        double bytesPerSec = static_cast<double>(bytesProceeded) * 10.0 / static_cast<double>(timeoutCount);

        if (currenStatus == statusReceiveBegan ||
                currenStatus == statusSendBegan) {
            //> conver speed
            QString metric = "Bytes/sec";
            if (bytesPerSec > 1500) {
                bytesPerSec /= 1000.0;
                metric = "KBytes/sec";
            }
            if (bytesPerSec > 1500) {
                bytesPerSec /= 1000.0;
                metric = "MBytes/sec";
            }
            //> set the text
            speedLabel->setText(QString::number(bytesPerSec, 'f', 3) + metric);
        }
        canUpdate = false;
    }
}
