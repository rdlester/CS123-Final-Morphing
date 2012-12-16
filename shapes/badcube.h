#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include "square.h"
#include "CS123Algebra.h"

class BadCube : public Shape
{
public:
    BadCube(int p);
    virtual ~BadCube();
    void build();

protected:
};

#endif // CUBE_H
