#ifndef CUBE_H
#define CUBE_H

#include <shape.h>

class Cube : public Shape
{
public:
    Cube(int p);
    Cube(int p, QString path);
    Cube(int p, int texId);
    ~Cube();
    void build();
};

#endif // CUBE_H
