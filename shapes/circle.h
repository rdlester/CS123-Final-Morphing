#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
#include "CS123Algebra.h"

class Circle : public Shape
{
public:
    Circle(int p1, int p2);
    ~Circle();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();

protected:
    Vector3* points;
};

#endif // CIRCLE_H
