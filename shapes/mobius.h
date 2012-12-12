#ifndef MOBIUS_H
#define MOBIUS_H

#include "shape.h"
#include "CS123Algebra.h"

class Mobius : public Shape
{
public:
    Mobius(int p1, int p2);
    virtual ~Mobius();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();
protected:
    Vector3* points;
    Vector3* normals;
};

#endif // MOBIUS_H
