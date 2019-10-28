#include "quitbutton.h"
#include "squaresize.h"
#include <QIcon>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QTouchEvent>
#include <QGraphicsScene>
#include <QDebug>

QuitButton::QuitButton() : QGraphicsPixmapItem(QIcon(":/img/application-exit.svg").pixmap(SquareSize(qRound(qApp->primaryScreen()->physicalDotsPerInch() / 25.40)*10))) {
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

QVariant QuitButton::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    QVariant ret = QGraphicsPixmapItem::itemChange(change,value);
    if (change == QGraphicsItem::ItemSceneChange) {
        QGraphicsScene * scene = value.value<QGraphicsScene*>();
        if (scene != NULL) setPos(scene->sceneRect().width() - boundingRect().width()-1,1);
    }
    return ret;
}

bool QuitButton::sceneEvent(QEvent *event) {
    bool ret = QGraphicsPixmapItem::sceneEvent(event);
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent * tevent = (QTouchEvent *)event;
        if (tevent->touchPointStates() & Qt::TouchPointReleased) {
            QList<QTouchEvent::TouchPoint> points = tevent->touchPoints();
            for (int i=0;i<points.count();i++) {
                if (sceneBoundingRect().contains(points[i].pos())) {
                    if (points[i].state() == Qt::TouchPointReleased) {
                        mouseReleaseEvent(NULL);
                        break;
                    }
                    else if (points[i].state() == Qt::TouchPointPressed) {
                        mousePressEvent(NULL);
                        break;
                    }
                }
            }
        }
        break;
    }
    default:
        break;
    }

    return ret;
}

void QuitButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPixmapItem::mousePressEvent(event);
    event->accept();
}

void QuitButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    qApp->quit();
}

