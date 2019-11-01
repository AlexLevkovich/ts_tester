#include "testscene.h"
#include "quitbutton.h"
#include "touchitem.h"
#include <QApplication>
#include <QScreen>
#include <QTouchEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QFontMetrics>
#include <QPainter>

TestScene::TestScene() : QGraphicsScene() {
    setSceneRect(qApp->primaryScreen()->geometry());
    setBackgroundBrush(QBrush(Qt::black));
    addItem(new QuitButton());
    centerInfoRect.setX(sceneRect().width()/3);
    centerInfoRect.setY(sceneRect().height()/3);
    centerInfoRect.setWidth(centerInfoRect.x());
    centerInfoRect.setHeight(centerInfoRect.y());
}

TestScene::~TestScene() {
}

void TestScene::adaptPainterFontSize(QPainter *painter,const QRectF & drawRect,const QString & text) {
     QRect fontBoundRect = painter->fontMetrics().boundingRect(drawRect.toRect(),Qt::TextDontClip|Qt::TextWordWrap,text);
     qreal xFactor = drawRect.width() / fontBoundRect.width();
     qreal yFactor = drawRect.height() / fontBoundRect.height();
     qreal factor = xFactor < yFactor ? xFactor : yFactor;
     QFont f = painter->font();
     f.setPointSizeF(f.pointSizeF()*factor);
     painter->setFont(f);
 }

void TestScene::drawBackground(QPainter *painter, const QRectF &rect) {
    QGraphicsScene::drawBackground(painter,rect);

    if (!centerInfoRect.intersects(rect)) return;

    painter->setRenderHint(QPainter::Antialiasing);
    QString text = QString::number(TouchItem::count());
    adaptPainterFontSize(painter,centerInfoRect,text);
    painter->drawText(centerInfoRect,Qt::AlignHCenter|Qt::AlignVCenter,text);
}

bool TestScene::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent * tevent = (QTouchEvent *)event;
        QList<QTouchEvent::TouchPoint> points = tevent->touchPoints();
        for (int i=0;i<points.count();i++) {
            QTouchEvent touchEvent(tevent->type());
            touchEvent.setWindow(tevent->window());
            touchEvent.setTarget(tevent->target());
            touchEvent.setDevice(tevent->device());
            touchEvent.setModifiers(tevent->modifiers());
            touchEvent.setTouchPointStates(points[i].state());
            touchEvent.setTimestamp(tevent->timestamp());
            touchEvent.setTouchPoints(QList<QTouchEvent::TouchPoint>()<<points[i]);

            QList<QGraphicsItem *> items = this->items((points[i].state() == Qt::TouchPointMoved)?points[i].lastScenePos():points[i].scenePos());
            if (items.count() > 0) {
                for (int j=0;j<items.count();j++) {
                    sendEvent(items[j],&touchEvent);
                }
            }
            else if (points[i].state() == Qt::TouchPointPressed) {
                sendEvent(createNewItem(points[i].scenePos()),&touchEvent);
            }
        }
        return true;
    }
    default:
        return QGraphicsScene::event(event);
    }
}

TouchItem * TestScene::createNewItem(const QPointF & pos) {
    TouchItem * item = new TouchItem(pos);
    connect(item,SIGNAL(destroyed()),SLOT(item_destroyed()));
    update(centerInfoRect);
    addItem(item);
    return item;
}

void TestScene::item_destroyed() {
    update(centerInfoRect);
}

void TestScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() != Qt::LeftButton) return;
    QList<QGraphicsItem *> items = this->items(mouseEvent->scenePos());
    if (items.count() > 0) {
        for (int j=0;j<items.count();j++) {
            sendEvent(items[j],mouseEvent);
        }
    }
    else sendEvent(createNewItem(mouseEvent->scenePos()),mouseEvent);
}

void TestScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() != Qt::LeftButton) return;
    QList<QGraphicsItem *> items = this->items(mouseEvent->scenePos());
    if (items.count() > 0) {
        for (int j=0;j<items.count();j++) {
            sendEvent(items[j],mouseEvent);
        }
    }
}

void TestScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QList<QGraphicsItem *> items = this->items(mouseEvent->lastScenePos());
    if (items.count() > 0) {
        for (int j=0;j<items.count();j++) {
            sendEvent(items[j],mouseEvent);
        }
    }
}
