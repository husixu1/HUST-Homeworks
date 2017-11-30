/// @file animations.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "animations.h"


void Animations::shakeLabel(QLabel *label, int startIndent, int endIndent, quint64 miliseconds) {
    QPropertyAnimation *shake = new QPropertyAnimation(label, "indent");
    shake->setEasingCurve(QEasingCurve::OutElastic);
    shake->setStartValue(startIndent);
    shake->setEndValue(endIndent);
    shake->setDuration(miliseconds);
    shake->start();
}
