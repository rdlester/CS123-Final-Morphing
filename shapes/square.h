#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"
#include "CS123Algebra.h"

class Square : public Shape
{
public:
    Square(int p);
    Square(int p, QString path);
    Square(int p, int texId);
    ~Square();
    void build();

protected:
};

#endif // SQUARE_H
