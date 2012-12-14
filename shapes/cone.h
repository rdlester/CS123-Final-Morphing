#ifndef CONE_H
#define CONE_H

#include "shape.h"
#include "circle.h"
#include "CS123Algebra.h"

class Cone : public Shape
{
public:
    Cone(int p);
    virtual ~Cone();
    void build();
protected:
};

#endif // CONE_H
