#ifndef CONE_H
#define CONE_H

#include "shape.h"
#include "circle.h"
#include "CS123Algebra.h"

class Cone : public Shape
{
public:
    Cone(int p1, int p2);
    virtual ~Cone();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();
protected:
    Circle* circle;
    Vector3* points;
    Vector3* normals;
};

#endif // CONE_H
