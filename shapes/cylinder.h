#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "circle.h"
#include "CS123Algebra.h"

class Cylinder : public Shape
{
public:
    Cylinder(int p);
    Cylinder(int p, QString path);
    Cylinder(int p, int texId);
    virtual ~Cylinder();
    void build();
protected:
};

#endif // CYLINDER_H
