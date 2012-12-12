#include "sphere.h"

Sphere::Sphere(int p1, int p2) : Shape()
{
    points = NULL;
    normals = NULL;
    setParams(p1, p2);
}

Sphere::~Sphere()
{
    delete[] points;
    delete[] normals;
}

void Sphere::draw()
{
    // top
    int i,j;
    Vector3 north = points[0];
    Vector3 northn = normals[0];
    Vector3 curr = points[1];
    Vector3 currn = normals[1];
    for (j = 1; j <= _p2; j++) {
        Vector3 next = points[j%_p2 + 1];
        Vector3 nextn = normals[j%_p2 + 1];
        glBegin(GL_TRIANGLES);
        glNormal3f(northn.x,northn.y,northn.z);
        glVertex3f(north.x,north.y,north.z);
        glNormal3f(nextn.x,nextn.y,nextn.z);
        glVertex3f(next.x,next.y,next.z);
        glNormal3f(currn.x,currn.y,currn.z);
        glVertex3f(curr.x,curr.y,curr.z);
        glEnd();
        curr = next;
        currn = nextn;
    }

    // center
    for (i = 0; i < _p1-2; i++) {
        for (j = 0; j < _p2; j++) {
            Vector3 ind1 = points[i*_p2+j+1];
            Vector3 ind1n = normals[i*_p2+j+1];
            Vector3 ind2 = points[i*_p2+((j+1)%_p2)+1];
            Vector3 ind2n = normals[i*_p2+((j+1)%_p2)+1];
            Vector3 ind3 = points[(i+1)*_p2+j+1];
            Vector3 ind3n = normals[(i+1)*_p2+j+1];
            Vector3 ind4 = points[(i+1)*_p2+((j+1)%_p2)+1];
            Vector3 ind4n = normals[(i+1)*_p2+((j+1)%_p2)+1];
            glBegin(GL_TRIANGLES);
            glNormal3f(ind1n.x,ind1n.y,ind1n.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glNormal3f(ind2n.x,ind2n.y,ind2n.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(ind3n.x,ind3n.y,ind3n.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();
            glBegin(GL_TRIANGLES);
            glNormal3f(ind2n.x,ind2n.y,ind2n.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(ind4n.x,ind4n.y,ind4n.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glNormal3f(ind3n.x,ind3n.y,ind3n.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();
        }
    }

    // bottom
    Vector3 south = points[1+(_p1-1)*_p2];
    Vector3 southn = normals[1+(_p1-1)*_p2];
    int start = 1+(_p1-2)*_p2;
    curr = points[start];
    currn = normals[start];
    for (j = 1; j <= _p2; j++) {
        Vector3 next = points[start+(j%_p2)];
        Vector3 nextn = normals[start+(j%_p2)];
        glBegin(GL_TRIANGLES);
        glNormal3f(southn.x,southn.y,southn.z);
        glVertex3f(south.x,south.y,south.z);
        glNormal3f(currn.x,currn.y,currn.z);
        glVertex3f(curr.x,curr.y,curr.z);
        glNormal3f(nextn.x,nextn.y,nextn.z);
        glVertex3f(next.x,next.y,next.z);
        glEnd();
        curr = next;
        currn = nextn;
    }
}

void Sphere::build()
{
    if(_p1 < 2) {
        _p1 = 2;
    }
    if(_p2 < 3) {
        _p2 = 3;
    }

    delete[] points;
    delete[] normals;
    points = new Vector3[2+(_p1-1)*_p2];
    normals = new Vector3[2+(_p1-1)*_p2];

    // north pole
    points[0] = Vector3(0,0.5,0);
    normals[0] = Vector3(0,1,0);
    float r = 0.5;
    int i,j;
    for (i = 0; i < (_p1-1); i++) {
        float phi = M_PI * (i+1)/(float)_p1;
        float xz = r*sin(phi);
        float y = r*cos(phi);
        for (j = 0; j < _p2; j++) {
            float theta = M_PI * 2 * j/(float)_p2;
            Vector3 p = Vector3(xz*cos(theta),y,xz*sin(theta));
            double h = sqrt(pow(p.x,2)+pow(p.y,2)+pow(p.z,2));
            points[i*_p2+j+1] = p;
            normals[i*_p2+j+1] = Vector3(p.x/h,p.y/h,p.z/h);
        }
    }
    //south pole
    points[1+(_p1-1)*_p2] = Vector3(0,-0.5,0);
    normals[1+(_p1-1)*_p2] = Vector3(0,-1,0);
}
