#include "touchmain.h"
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
#include "touchemulator.h"

class TestScene : public QGraphicsScene {
public:
    TestScene(bool test_mode) : QGraphicsScene() {
        setSceneRect(qApp->primaryScreen()->geometry());
        setBackgroundBrush(QBrush(Qt::black));
        addItem(new QuitButton());
        this->test_mode = test_mode;
        middle_count = 0;
        centerInfoRect.setX(sceneRect().width()/3);
        centerInfoRect.setY(sceneRect().height()/3);
        centerInfoRect.setWidth(centerInfoRect.x());
        centerInfoRect.setHeight(centerInfoRect.y());
    }

    ~TestScene() {
        deleteTouches();
    }

protected:
    void adaptPainterFontSize(QPainter *painter,const QRectF & drawRect,const QString & text) {
         QRect fontBoundRect = painter->fontMetrics().boundingRect(drawRect.toRect(),Qt::TextDontClip|Qt::TextWordWrap,text);
         qreal xFactor = drawRect.width() / fontBoundRect.width();
         qreal yFactor = drawRect.height() / fontBoundRect.height();
         qreal factor = xFactor < yFactor ? xFactor : yFactor;
         QFont f = painter->font();
         f.setPointSizeF(f.pointSizeF()*factor);
         painter->setFont(f);
     }

    void drawBackground(QPainter *painter, const QRectF &rect) {
        QGraphicsScene::drawBackground(painter,rect);

        if (!centerInfoRect.intersects(rect)) return;

        painter->setRenderHint(QPainter::Antialiasing);
        QString text = QString::number(TouchItem::count());
        adaptPainterFontSize(painter,centerInfoRect,text);
        painter->drawText(centerInfoRect,Qt::AlignHCenter|Qt::AlignVCenter,text);
    }

    bool event(QEvent *event) {
        bool ret = QGraphicsScene::event(event);
        switch (event->type()) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        {
            QTouchEvent * tevent = (QTouchEvent *)event;
            QList<QTouchEvent::TouchPoint> points = tevent->touchPoints();
            for (int i=0;i<points.count();i++) {
                if (points[i].state() == Qt::TouchPointReleased) {
                    mouseReleaseEvent(points[i].scenePos());
                }
                else if (points[i].state() == Qt::TouchPointPressed) {
                    mousePressEvent(points[i].scenePos(),event);
                }
                else if (points[i].state() == Qt::TouchPointMoved) {
                    QList<QGraphicsItem *> items = this->items(points[i].lastScenePos());
                    for (int j=0;j<items.count();j++) {
                        if (items[j]->type() == QGraphicsPixmapItem::Type) continue;
                        items[j]->setPos(points[i].scenePos());
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

    void mousePressEvent(const QPointF & pos,QEvent * event) {
        if (items(pos).count() > 0) return;
        TouchItem * item = new TouchItem(pos);
        update(centerInfoRect);
        addItem(item);
        this->event(event);
    }

    void mouseReleaseEvent(const QPointF & pos) {
        QList<QGraphicsItem *> items = this->items(pos);
        if (this->items(pos).count() <= 0) return;
        for (int i=0;i<items.count();i++) {
            if (items[i]->type() == QGraphicsPixmapItem::Type) continue;
            delete items[i];
            update(centerInfoRect);
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        if (!test_mode) QGraphicsScene::mouseMoveEvent(mouseEvent);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        QGraphicsScene::mousePressEvent(mouseEvent);
        if (!test_mode && mouseEvent->button() == Qt::LeftButton) mousePressEvent(mouseEvent->scenePos(),mouseEvent);
        else if (test_mode && mouseEvent->button() == Qt::MiddleButton) {
            middle_count++;
            if (middle_count == 1) {
                QPointF pos = mouseEvent->scenePos();
                start_x = pos.x();
                start_y = pos.y();
                TouchEmulator::newTouch(QPointF(pos.x()-200,pos.y()));
                TouchEmulator::newTouch(QPointF(pos.x()+200,pos.y()));
            }
            else if (middle_count == 2) {
                middle_count = 0;
                deleteTouches();
            }
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        if (!test_mode) mouseReleaseEvent(mouseEvent->scenePos());
    }

    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
        QGraphicsScene::wheelEvent(wheelEvent);
        if (!test_mode) return;

        QList<QGraphicsItem *> items = this->items();
        TouchItem * item;
        qreal x;
        for (int i=0;i<items.count();i++) {
            if (items[i]->type() == QGraphicsPixmapItem::Type) continue;
            item = (TouchItem *)items[i];
            if (item->id() == 0) {
                x = item->scenePos().x()+(((wheelEvent->delta()<0)?-1:1)*20);
                if (x > 0 && x < (start_x - TouchItem::dimention())) TouchEmulator::move(item->id(),QPointF(x,start_y));
            }
            else if (item->id() == 1) {
                x = item->scenePos().x()+(((wheelEvent->delta()<0)?1:-1)*20);
                if (x < sceneRect().width() && x > (start_x + TouchItem::dimention())) TouchEmulator::move(item->id(),QPointF(x,start_y));
            }
        }
    }

private:
    void deleteTouches() {
        if (!test_mode) return;

        TouchItem * item;
        QList<QGraphicsItem *> items = this->items();
        for (int j=0;j<items.count();j++) {
            if (items[j]->type() == QGraphicsPixmapItem::Type) continue;
            item = (TouchItem *)items[j];
            TouchEmulator::unTouch(item->id());
        }
    }

    bool test_mode;
    int middle_count;
    qreal start_x;
    qreal start_y;
    QRectF centerInfoRect;
    int centerFontSize;
};

TouchMain::TouchMain(bool test_mode,QWidget *parent) : QGraphicsView(parent) {
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setCacheMode(QGraphicsView::CacheNone);
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    viewport()->setMouseTracking(true);

    scene = new TestScene(test_mode);
    setScene(scene);
    showFullScreen();
}

TouchMain::~TouchMain() {
    delete scene;
}

