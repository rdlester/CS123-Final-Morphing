#ifndef STRAIGHTLINE_H
#define STRAIGHTLINE_H

#include "curve.h"
class StraightLine : public Curve
{
public:
    StraightLine();
    virtual ~StraightLine() {};

    void addPoint(float x, float y);
    void deleteActivePoint();
    void moveActivePoint(float dx, float dy);
    void drawNormals();
};

#endif // STRAIGHTLINE_H
