#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include "square.h"
#include "CS123Algebra.h"

class BadCube : public Shape
{
public:
    BadCube(int p);
    BadCube(int p, QString path);
    BadCube(int p, int texId);
    virtual ~BadCube();
    void build();

protected:
};

#endif // CUBE_H
