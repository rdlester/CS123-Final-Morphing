#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"
#include "CS123Algebra.h"

class Square : public Shape
{
public:
    Square(int p);
    ~Square();
    void build();

protected:
};

#endif // SQUARE_H
