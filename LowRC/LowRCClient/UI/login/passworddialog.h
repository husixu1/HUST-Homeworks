/// @file passworddialog.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "animations.h"
#include "clientcore.h"
#include "baseconnector.h"
#include "protocolconstant.h"

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSharedPointer>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog {
    Q_OBJECT

public:
    /// @brief the three phase a password changing will go through
    enum Phase {
        phaseEmail,
        phaseQuestion,
        phasePassword,
    };

private:
    Ui::PasswordDialog *ui;

    Phase currentPhase;

    /// @brief set the safe questoin
    void setQuestion(QString question);

    /// @brief dispaly an error message on status bar
    void setStatusBarError(QString error);

    /// @brief the email whoes password the user want to recover
    QString userEmail;
    /// @brief the validation key
    quint64 validateKey;

    /// @brief the first phase of changing the password, waiting for UI event
    void passwdChangePhase1(QString);
    /// @brief the second phase of changing the password, waiting for UI event
    void passwdChangePhase2(QString);
    /// @brief the third phase of changing the password, waiting for UI event
    void passwdChangePhase3(QString);

    QSharedPointer<OperationDescriptor> passwdChangeOperation;
public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();

    void setPhase(Phase phase);

private slots:
    /// @brief deal with submit button clicked data
    void forwardSubmitbuttonClicked();

    /// @brief when the 1st phase replyed
    void passwdPhase1Replyed(QByteArray);
    /// @brief when the 1st phase suffers a send/receive error
    void passwdPhase1Error(OperationDescriptor::Error);

    /// @brief when the 2nd phase replyed
    void passwdPhase2Replyed(QByteArray);
    /// @brief when the 2nd phase suffers a send/receive error
    void passwdPhase2Error(OperationDescriptor::Error);

    /// @brief when the 3rd phase replyed
    void passwdPhase3Replyed(QByteArray);
    /// @brief when the 3rd phase suffers a send/receive error
    void passwdPhase3Error(OperationDescriptor::Error);

signals:
    /// @brief when submit button is clicked
    void submitButtonClicked(QString data);

    /// @brief send the password changing data
    void trySendData(QSharedPointer<OperationDescriptor>,
            QHostAddress, quint16, quint64, BaseConnector::DataType, QByteArray);

};

#endif // PASSWORDDIALOG_H
