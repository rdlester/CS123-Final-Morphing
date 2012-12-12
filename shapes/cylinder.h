#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "circle.h"
#include "CS123Algebra.h"

class Cylinder : public Shape
{
public:
    Cylinder(int p1, int p2);
    virtual ~Cylinder();
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

#endif // CYLINDER_H
