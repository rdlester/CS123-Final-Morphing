#include "shape.h"

Shape::Shape()
{
// Nothing to see here
}

Shape::~Shape()
{
// nor here
}

void Shape::setParams(int p1, int p2)
{
    if(_p1 != p1 || _p2 != p2) {
        _p1 = p1;
        _p2 = p2;
        build();
    }
}
