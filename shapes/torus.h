#ifndef TORUS_H
#define TORUS_H

#include "shape.h"
#include "CS123Algebra.h"

class Torus : public Shape
{
public:
    Torus(int p1, int p2);
    virtual ~Torus();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();
protected:
    Vector3* points;
    Vector3* normals;
};

#endif // TORUS_H
