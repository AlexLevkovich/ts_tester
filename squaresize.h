#ifndef SQUARESIZE_H
#define SQUARESIZE_H

#include <QSize>
#include <QRectF>

class SquareSize : public QSize {
public:
    SquareSize(int dimension);
};

class SquareRectF : public QRectF {
public:
    SquareRectF(int dimension);
};


#endif // SQUARESIZE_H
