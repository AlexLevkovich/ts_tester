#include "touchmain.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TouchMain w;
    w.show();
    return a.exec();
}
