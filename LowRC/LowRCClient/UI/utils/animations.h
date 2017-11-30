/// @file animations.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>
#include <QEasingCurve>

namespace Animations {

/// @brief make the label shake, usually used when error occurred
void shakeLabel(QLabel *label, int startIndent, int endIndent, quint64 miliseconds);
}

#endif // ANIMATIONS_H
