#ifndef QUITBUTTON_H
#define QUITBUTTON_H

#include <QGraphicsPixmapItem>

class QuitButton : public QGraphicsPixmapItem {
public:
    QuitButton();

protected:
    bool sceneEvent(QEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

};

#endif // QUITBUTTON_H
