#ifndef TOUCHITEM_H
#define TOUCHITEM_H

#include <QGraphicsEllipseItem>
#include <QObject>

class QGraphicsSceneMouseEvent;

class TouchItem : public QObject,public QGraphicsEllipseItem {
    Q_OBJECT
public:
    TouchItem(const QPointF & pos);
    ~TouchItem();
    int id() const;
    static int count();
    static qreal dimention();

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    bool sceneEvent(QEvent *event);
private:
    void generateID();

    int m_id;
    static int m_count;
    static QList<bool> touched;

};

#endif // TOUCHITEM_H
