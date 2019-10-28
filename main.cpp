#include "touchmain.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TouchMain w(argc >= 2 && !strcmp(argv[1],"--test"));
    w.show();
    return a.exec();
}
