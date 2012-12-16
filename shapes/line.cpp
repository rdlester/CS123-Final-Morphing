#include "line.h"

Line::Line(int p) : Shape(0)
{
    setParams(p);
}

Line::~Line()
{

}

void Line::build()
{
    Shape::build();

    int i,j;
    for (i = 0; i < _p; i++) {
        float y = 0.5 - (float)i/(float)(_p-1);
        for (j = 0; j < _p; j++) {
            Matrix4x4 rot = getRotYMat(2*M_PI*j/(float)(_p-1));
            Vector3 n = Vector3(0,0,-1);
            n = rot*n;
            _vertices[i*_p+j] = Vector3(0,y,0);
            _normals[i*_p+j] = n;
        }
    }
}
