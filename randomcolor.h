#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include <QColor>
#include <QPen>
#include <QBrush>

class RandomColor {
public:
    static const QColor color();
    static const QPen pen();
    static const QBrush brush();

private:
    static float currentHue;
};

#endif // RANDOMCOLOR_H
