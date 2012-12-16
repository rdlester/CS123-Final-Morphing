#ifndef BSPLINE_H
#define BSPLINE_H

#include <curve.h>

class Bspline : public Curve
{
protected:
        void computeSegment(Point p1, Point p2, Point p3, Point p4, int levelOfDetail);

public:
        void draw(int levelOfDetail);
        void drawNormals();
        Bspline() {};
        virtual ~Bspline() {};

};

#endif // BSPLINE_H
