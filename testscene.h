#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <QGraphicsScene>

class QPainter;
class QEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class TouchItem;

class TestScene : public QGraphicsScene {
    Q_OBJECT
public:
    TestScene();
    ~TestScene();

protected:
    void adaptPainterFontSize(QPainter *painter,const QRectF & drawRect,const QString & text);
    void drawBackground(QPainter *painter, const QRectF &rect);
    bool event(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    TouchItem * createNewItem(const QPointF & pos);

private slots:
    void item_destroyed();

private:
    QRectF centerInfoRect;
    int centerFontSize;
};

#endif // TESTSCENE_H
