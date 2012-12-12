#include "torus.h"

Torus::Torus(int p1, int p2) : Shape()
{
    points = NULL;
    normals = NULL;
    setParams(p1,p2);
}

Torus::~Torus()
{
    delete[] points;
    delete[] normals;
}

void Torus::draw()
{
    int i,j;
    for (i = 0; i < _p1; i++) {
        for (j = 0; j < _p2; j++) {
            Vector3 ind1,ind1n,ind2,ind2n,ind3,ind3n,ind4,ind4n;
            ind1 = points[i*_p2+j];
            ind1n = normals[i*_p2+j];
            ind2 = points[i*_p2+((j+1)%_p2)];
            ind2n = normals[i*_p2+((j+1)%_p2)];
            ind3 = points[((i+1)%_p1)*_p2+j];
            ind3n = normals[((i+1)%_p1)*_p2+j];
            ind4 = points[((i+1)%_p1)*_p2+((j+1)%_p2)];
            ind4n = normals[((i+1)%_p1)*_p2+((j+1)%_p2)];

            glBegin(GL_TRIANGLES);
            glNormal3f(ind1n.x,ind1n.y,ind1n.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glNormal3f(ind3n.x,ind3n.y,ind3n.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glNormal3f(ind2n.x,ind2n.y,ind2n.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glEnd();
            glBegin(GL_TRIANGLES);
            glNormal3f(ind2n.x,ind2n.y,ind2n.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(ind3n.x,ind3n.y,ind3n.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glNormal3f(ind4n.x,ind4n.y,ind4n.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glEnd();
        }
    }
}

void Torus::build()
{
    if(_p1 < 3) {
        _p1 = 3;
    }
    if(_p2 < 3) {
        _p2 = 3;
    }

    delete[] points;
    delete[] normals;
    points = new Vector3[_p1*_p2];
    normals = new Vector3[_p1*_p2];

    float R = 0.3; // distance from center to center of tube
    float r = 0.2; // distance from center of tube to edge of tube
    int j,i;
    for (i = 0; i < _p1; i++) { // internal phi
        float phi = M_PI * 2 * (i/(float)_p1);
        float xz = R+r*cos(phi);
        float y = r*sin(phi);
        for (j = 0; j < _p2; j++) { // external theta ((R+rcos(phi))cos(theta),rsin(phi),(R+rcos(phi))sin(theta))
            float theta = M_PI * 2 * (j/(float)_p2);
            points[i*_p2+j] = Vector3(xz*cos(theta),y,xz*sin(theta));
            normals[i*_p2+j] = Vector3(cos(phi)*cos(theta),sin(phi),cos(phi)*sin(theta));
        }
    }
}
