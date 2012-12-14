#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
#include "CS123Algebra.h"

class Circle : public Shape
{
public:
    Circle(int p);
    ~Circle();
    void build();

protected:
};

#endif // CIRCLE_H
