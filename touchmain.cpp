#include "touchmain.h"
#include "testscene.h"

TouchMain::TouchMain(QWidget *parent) : QGraphicsView(parent) {
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    viewport()->setMouseTracking(true);

    scene = new TestScene();
    setScene(scene);
    showFullScreen();
}

TouchMain::~TouchMain() {
    delete scene;
}

