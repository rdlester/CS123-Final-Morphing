#include "cone.h"

Cone::Cone(int p) : Shape(0)
{
    setParams(p);
}
Cone::Cone(int p, QString path) : Shape(path)
{
    setParams(p);
}
Cone::Cone(int p, int texId) : Shape(texId)
{
    setParams(p);
}

Cone::~Cone()
{
}

void Cone::build()
{
    Shape::build();

    float r = 0.5;
    float ynorm = 1/sqrt(5);
    float xznorm = 2/sqrt(5);
    int i,j;
    // body
    for (i = 0; i < _p-1; i++) { //level _p-1 is cap
        float currY = r - 2.f * r * (i/(float)(_p-2));
        for (j = 0; j < _p; j++) {
            float theta = M_PI * 2.f * (1 - j/(float)(_p-1));
            float x = cos(theta);
            float z = sin(theta);
            float bodyWidth = i*r/(float)(_p-2);
            _vertices[i*_p + j] = Vector3(x*bodyWidth,currY,z*bodyWidth);
            _normals[i*_p + j] = Vector3(xznorm*x, ynorm, xznorm*z);
        }
    }
    // cap
    int ip = (_p-1)*_p;
    for (j = 0; j < _p; j++) {
        _vertices[ip+j] = Vector3(0,-r,0);
        _normals[ip+j] = Vector3(0,-1,0);
    }
}
