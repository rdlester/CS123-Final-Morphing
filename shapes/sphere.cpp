#include "sphere.h"

Sphere::Sphere(int p) : Shape(0)
{
    setParams(p);
}
Sphere::Sphere(int p, QString path) : Shape(path)
{
    setParams(p);
}
Sphere::Sphere(int p, int texId) : Shape(texId)
{
    setParams(p);
}

Sphere::~Sphere()
{
}

void Sphere::build()
{
    Shape::build();

    int i,j;
    for (i = 0; i < _p; i++) {
        // Vertical tilt
        float phi = M_PI * i/(float)(_p-1);
        float xz = 0.5*sin(phi);
        float y = 0.5*cos(phi);
        for (j = 0; j < _p; j++) {
            // tilt in xz plane
            float theta = M_PI * 2.f * (1.f - j/(float)(_p-1));
            Vector3 p = Vector3(xz*cos(theta),y,xz*sin(theta));
            double h = sqrt(pow(p.x,2)+pow(p.y,2)+pow(p.z,2));
            _vertices[i*_p+j] = p;
            _normals[i*_p+j] = Vector3(p.x/h,p.y/h,p.z/h);
        }
    }
}
