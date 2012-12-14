#include "torus.h"

Torus::Torus(int p) : Shape()
{
    setParams(p);
}

Torus::~Torus()
{
}

void Torus::build()
{
    Shape::build();
//    if(_p1 < 3) {
//        _p1 = 3;
//    }
//    if(_p2 < 3) {
//        _p2 = 3;
//    }

//    delete[] points;
//    delete[] normals;
//    points = new Vector3[_p1*_p2];
//    normals = new Vector3[_p1*_p2];

//    float R = 0.3; // distance from center to center of tube
//    float r = 0.2; // distance from center of tube to edge of tube
//    int j,i;
//    for (i = 0; i < _p1; i++) { // internal phi
//        float phi = M_PI * 2 * (i/(float)_p1);
//        float xz = R+r*cos(phi);
//        float y = r*sin(phi);
//        for (j = 0; j < _p2; j++) { // external theta ((R+rcos(phi))cos(theta),rsin(phi),(R+rcos(phi))sin(theta))
//            float theta = M_PI * 2 * (j/(float)_p2);
//            points[i*_p2+j] = Vector3(xz*cos(theta),y,xz*sin(theta));
//            normals[i*_p2+j] = Vector3(cos(phi)*cos(theta),sin(phi),cos(phi)*sin(theta));
//        }
//    }
}
