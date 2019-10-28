#ifndef TOUCHEMULATOR_H
#define TOUCHEMULATOR_H

#include <QPointF>
#include <QByteArray>

class TouchEmulator {
public:
    static void newTouch(const QPointF & point);
    static void unTouch(int id);
    static void move(int id,const QPointF & point);

private:
    static void writeToDevice(const QByteArray & data);
};

#endif // TOUCHEMULATOR_H
