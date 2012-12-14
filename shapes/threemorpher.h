#ifndef THREEMORPHER_H
#define THREEMORPHER_H

#include <shape.h>

class ThreeMorpher : public Shape
{
public:
    ThreeMorpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn,
                 Vector3* cv, Vector3* cn, int p);
    virtual ~ThreeMorpher();
    virtual void build();

    //! Interpolates the two shapes to mix given by intersection of t and simplex
    void morphTo(Vector3 t);
protected:
    //! Shapes the morpher varies between
    Vector3* _shapeAv;
    Vector3* _shapeAn;
    Vector3* _shapeBv;
    Vector3* _shapeBn;
    Vector3* _shapeCv;
    Vector3* _shapeCn;
};

#endif // THREEMORPHER_H
