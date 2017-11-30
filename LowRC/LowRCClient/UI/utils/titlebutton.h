#ifndef TITLEBUTTON_H
#define TITLEBUTTON_H

#include "clickablelabel.h"

#include <QWidget>

namespace Ui {
class TitleButton;
}

class TitleButton : public QWidget {
    Q_OBJECT

public:
    explicit TitleButton(QWidget *parent = 0);
    ~TitleButton();

    /// @brief the button to use
    enum Button {
        CloseButton = 1,
        minimizeButton = 2,
        settingButon = 4
    };

    /// @brief only use some of the buttons
    void onlyUse(Button buttons);

    ClickableLabel *getCloseButton();
    ClickableLabel *getMinimizeButton();
    ClickableLabel *getSettingButon();
private:
    Ui::TitleButton *ui;
};

#endif // TITLEBUTTON_H
