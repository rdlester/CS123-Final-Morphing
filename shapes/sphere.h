#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "CS123Algebra.h"

class Sphere : public Shape
{
public:
    Sphere(int p1, int p2);
    virtual ~Sphere();
    void draw();
    void build();
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();
protected:
    Vector3* points;
    Vector3* normals;
};

#endif // SPHERE_H
