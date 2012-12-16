#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
#include "CS123Algebra.h"

class Circle : public Shape
{
public:
    Circle(int p);
    Circle(int p, QString path);
    Circle(int p, int texId);
    ~Circle();
    void build();

protected:
};

#endif // CIRCLE_H
