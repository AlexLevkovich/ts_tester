#ifndef TOUCHITEM_H
#define TOUCHITEM_H

#include <QGraphicsEllipseItem>

class TouchItem : public QGraphicsEllipseItem {
public:
    TouchItem(const QPointF & pos);
    ~TouchItem();
    int id() const;
    static int count();
    static qreal dimention();

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

private:
    void generateID();

    int m_id;
    static int m_count;
    static QList<bool> touched;
};

#endif // TOUCHITEM_H
