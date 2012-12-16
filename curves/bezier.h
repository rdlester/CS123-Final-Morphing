#ifndef BEZIER_H
#define BEZIER_H

#include "curve.h"
#include "math/CS123Algebra.h"

class Bezier : public Curve
{
public:
    Bezier() {};
    virtual ~Bezier() {};
    void draw(int levelOfDetail);
    void drawNormals();
    Point deCasteljau(float u, QVector<Point> points, QVector<Vector2>* normals);
};

#endif // BEZIER_H
