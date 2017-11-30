/// @file userinfodisplay.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef USERINFODISPLAY_H
#define USERINFODISPLAY_H

#include <QWidget>
#include <QMouseEvent>
#include <QtDebug>

namespace Ui {
class UserInfoDisplay;
}

class UserInfoDisplay : public QWidget {
    Q_OBJECT

private:
    Ui::UserInfoDisplay *ui;

    /// @brief some extra data
    QString userEmail;

protected:
    /// @brief on mouse pressed
    virtual void mousePressEvent(QMouseEvent *event) override;

    /// @brief mouse enter
    virtual void enterEvent(QEvent *event) override;
    /// @brief mouse leave
    virtual void leaveEvent(QEvent *event) override;

public:
    explicit UserInfoDisplay(QWidget *parent = 0);
    ~UserInfoDisplay();

    /// @brief set/update username
    void setUserName(QString username);

    /// @brief set/update message
    void setMessage(QString message);

    /// @brief set the icon
    /// @param[in] icon the base64 encoded icon file
    void setIcon(QString icon);

    /// @brief get the email of the user
    QString getUserEmail() const;

    /// @brief set the Email of the user
    void setUserEmail(const QString &value);

signals:
    void userInfoPressed(QString userEmail);
};

#endif // USERINFODISPLAY_H
