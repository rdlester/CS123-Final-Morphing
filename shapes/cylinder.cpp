#include "cylinder.h"

Cylinder::Cylinder(int p) : Shape()
{
    setParams(p);
}

Cylinder::~Cylinder()
{
}

void Cylinder::build()
{
    Shape::build();

    float r = 0.5;

    int i,j;

    // top point: i = 0;
    for (j = 0; j < _p; j++) {
        _vertices[j] = Vector3(0,r,0);
        _normals[j] = Vector3(0,1,0);
    }
    // body
    for (i = 1; i < _p-1; i++) {
        float currY = r - 2.f*r*(i/(float)(_p-2)); // it's p-2 b/c 0 and p-1 levels are caps
        for (j = 0; j < _p; j++) {
            float theta = M_PI * 2.f * (1 - j/(float)(_p-1));
            _vertices[i*_p + j] = Vector3(r*cos(theta),currY,r*sin(theta));
            _normals[i*_p + j] = Vector3(cos(theta),0,sin(theta));
        }
    }
    // bottom point: i = _p-1;
    int ip = (_p-1)*_p;
    for (j = 0; j < _p; j++) {
        _vertices[ip + j] = Vector3(0,-r,0);
        _normals[ip + j] = Vector3(0,-1,0);
    }
}
