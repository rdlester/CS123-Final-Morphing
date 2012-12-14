#ifndef MORPHER_H
#define MORPHER_H

#include <shape.h>

class Morpher : public Shape
{
public:
    Morpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn, int p);
    virtual ~Morpher();
    virtual void build();

    //! Interpolates the two shapes to mix given by t (in [0,1])
    void morphTo(float t);
protected:
    //! Shapes the morpher varies between
    Vector3* _shapeAv;
    Vector3* _shapeAn;
    Vector3* _shapeBv;
    Vector3* _shapeBn;
};

#endif // MORPHER_H
