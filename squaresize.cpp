#include "squaresize.h"

SquareSize::SquareSize(int dimension) : QSize(dimension,dimension) {}

SquareRectF::SquareRectF(int dimension) : QRectF(-dimension/2,-dimension/2,dimension,dimension) {}

