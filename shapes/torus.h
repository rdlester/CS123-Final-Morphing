#ifndef TORUS_H
#define TORUS_H

#include "shape.h"
#include "CS123Algebra.h"

class Torus : public Shape
{
public:
    Torus(int p);
    virtual ~Torus();
    void build();
protected:
};

#endif // TORUS_H
