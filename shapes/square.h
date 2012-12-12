#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"
#include "CS123Algebra.h"

class Square : public Shape
{
public:
    Square(int p1, int p2);
    ~Square();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();

protected:
    Vector3* points;
};

#endif // SQUARE_H
