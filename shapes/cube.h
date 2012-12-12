#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include "square.h"
#include "CS123Algebra.h"

class Cube : public Shape
{
public:
    Cube(int p1, int p2);
    virtual ~Cube();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();

protected:
    Square* square;
};

#endif // CUBE_H
