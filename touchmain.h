#ifndef TOUCHMAIN_H
#define TOUCHMAIN_H

#include <QGraphicsView>

class QGraphicsScene;

class TouchMain : public QGraphicsView {
    Q_OBJECT

public:
    TouchMain(bool test_mode = false,QWidget *parent = nullptr);
    ~TouchMain();

private:
    QGraphicsScene * scene;
};
#endif // TOUCHMAIN_H
