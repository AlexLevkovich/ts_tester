#include "randomcolor.h"
#include <cmath>

float RandomColor::currentHue = 0.0;

const QColor RandomColor::color() {
    QColor color = QColor::fromHslF(currentHue, 1.0, 0.5);
    currentHue += 0.618033988749895f;
    currentHue = std::fmod(currentHue,1.0f);
    return color;
}

const QPen RandomColor::pen() {
    return QPen(color());
}

const QBrush RandomColor::brush() {
    return QBrush(color());
}
