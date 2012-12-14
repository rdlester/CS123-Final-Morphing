#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "CS123Algebra.h"

class Sphere : public Shape
{
public:
    Sphere(int p);
    virtual ~Sphere();
    void build();
protected:
};

#endif // SPHERE_H
