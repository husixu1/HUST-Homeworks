/// @file fileinfodisplay.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef FILEINFODISPLAY_H
#define FILEINFODISPLAY_H

#include "operationdescriptor.h"
#include "clickablelabel.h"
#include <QWidget>

namespace Ui {
class FileInfoDisplay;
}

class FileInfoDisplay : public QWidget {
    Q_OBJECT

public:
    enum Status {
        statusReceiveInitial,
        statusReceiveBegan,
        statusReceiveFinished,
        statusSendInitial,
        statusSendBegan,
        statusSenFinished,
    };

private:
    Ui::FileInfoDisplay *ui;

    /// @brief the currens status of the label
    Status currenStatus;
    /// @brief a pool that stores all labels in omnilayout
    QVector<QWidget *> omniLayoutWidgets;
    /// @brief the label displaying current speed (if has)
    QLabel *speedLabel;

    /// @brief set the correspongding operation of this object
    /// this is used for fileAccepted and fileDeclined info
    QSharedPointer<OperationDescriptor> correspondingOperation;

    /// @brief all the bytes that has been proceeded
    quint64 bytesProceeded;
    /// @brief the total bytes to send/receive
    quint64 totalBytes;

    /// @brief updates every 0.1 second ( for updating speed )
    QTimer speedTimer;
    /// @brief the timeout count
    /// (the file will be received within 585 million centries so don't worry about 64bit uint)
    quint64 timeoutCount;
    /// @brief if the progress and speed can update
    bool canUpdate;
public:
    explicit FileInfoDisplay(QWidget *parent = 0);
    ~FileInfoDisplay();

    /// @brief set the status of this fileinfo display
    void setStatus(Status status);

    /// @brief set the file name
    void setFileName(QString filename);
    /// @brief set the icon of the file
    void setFileIcon(QIcon fileIcon);
    /// @brief set the progress of the file transger
    /// this will also set the speed display if available
    void setProgress(quint64 bytesProceeded);

    /// @brief set the corresponding operation of this object
    /// the set operation will be used in fileAccepted and fileDeclined signal
    void setCorrespondingOperation(QSharedPointer<OperationDescriptor> value);

    /// @brief set total bytes to send/receive
    void setTotalBytes(const quint64 &value);

private slots:
    void speedTimerTimedOut();

signals:
    /// @brief the accept label is clicked
    void fileAccecpted(QSharedPointer<OperationDescriptor> operation);
    /// @brief the decline label is clicked
    void fileDeclined(QSharedPointer<OperationDescriptor> operation);
};

#endif // FILEINFODISPLAY_H
