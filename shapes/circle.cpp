#include "circle.h"

Circle::Circle(int p) : Shape(0)
{
    setParams(p);
}
Circle::Circle(int p, QString path) : Shape(path)
{
    setParams(p);
}
Circle::Circle(int p, int texId) : Shape(texId)
{
    setParams(p);
}

Circle::~Circle()
{
}

void Circle::build()
{
    Shape::build();

    float r = 0.5;
    int i,j;
    for (i = 0; i < _p; i++) {
        float currR = r * (1.f-(i/(float)(_p-1)));
        for (j = 0; j < _p; j++) {
            float theta = M_PI * 2.f * (1.f - j/(float)(_p-1));
            _vertices[i*_p+j] = Vector3(currR*cos(theta),currR*sin(theta),0);
            _normals[i*_p+j] = Vector3(0,0,1);
        }
    }
}
