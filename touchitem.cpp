#include "touchitem.h"
#include "squaresize.h"
#include <QApplication>
#include <QScreen>
#include <QPen>
#include <QBrush>
#include <QTouchEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "randomcolor.h"

int TouchItem::m_count = 0;
QList<bool> TouchItem::touched;

TouchItem::TouchItem(const QPointF & pos) : QObject(NULL), QGraphicsEllipseItem(SquareRectF(TouchItem::dimention())) {
    setAcceptTouchEvents(true);
    setPen(QPen(Qt::black));
    setBrush(RandomColor::brush());
    setPos(pos);
    m_id = -1;
}

TouchItem::~TouchItem() {
    if (m_id >= 0) {
        m_count--;
        touched[m_id] = false;
    }
}

qreal TouchItem::dimention() {
    return qRound(qApp->primaryScreen()->physicalDotsPerInch() / 25.40)*16;
}

int TouchItem::count() {
    return m_count;
}

void TouchItem::generateID() {
    m_count++;
    for (int i=0;i<touched.count();i++) {
        if (!touched[i]) {
            m_id = i;
            touched[i] = true;
            return;
        }
    }
    m_id = touched.count();
    touched.append(true);
}

int TouchItem::id() const {
    return m_id;
}

QVariant TouchItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    QVariant ret = QGraphicsEllipseItem::itemChange(change,value);
    if (change == QGraphicsItem::ItemSceneChange) {
        if (value.value<QGraphicsScene*>() != NULL) generateID();
    }
    return ret;
}

bool TouchItem::sceneEvent(QEvent *event) {
    switch (event->type()) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        {
            QTouchEvent * tevent = (QTouchEvent *)event;
            QList<QTouchEvent::TouchPoint> points = tevent->touchPoints();
            for (int i=0;i<points.count();i++) {
                if (points[i].state() == Qt::TouchPointReleased) {
                    deleteLater();
                }
                else if (points[i].state() == Qt::TouchPointMoved) {
                    setPos(points[i].scenePos());
                }
            }
            break;
        }
        default:
            break;
    }
    return QGraphicsEllipseItem::sceneEvent(event);
}

void TouchItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
    deleteLater();
}

void TouchItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
   setPos(mouseEvent->scenePos());
}
