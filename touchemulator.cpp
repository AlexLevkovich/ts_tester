#include "touchemulator.h"
#include <QFile>
#include <QDebug>

void TouchEmulator::writeToDevice(const QByteArray & data) {
    QFile file("/dev/virtual_ts");
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "Cannot open /dev/virtual_ts!";
        return;
    }
    if (file.write(data) != data.length()) {
        qCritical() << "Cannot write to /dev/virtual_ts!";
        return;
    }
}

void TouchEmulator::newTouch(const QPointF & point) {
    writeToDevice(QByteArray("TOUCH ")+QByteArray::number((int)point.x())+" "+QByteArray::number((int)point.y())+"\n");
}

void TouchEmulator::unTouch(int id) {
    writeToDevice(QByteArray("UNTOUCH ")+QByteArray::number(id)+"\n");
}

void TouchEmulator::move(int id,const QPointF & point) {
    writeToDevice(QByteArray("MOVE ")+QByteArray::number(id)+" "+QByteArray::number((int)point.x())+" "+QByteArray::number((int)point.y())+"\n");
}
