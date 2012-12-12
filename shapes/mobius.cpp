#include "mobius.h"

Mobius::Mobius(int p1, int p2) : Shape()
{
    points = NULL;
    normals = NULL;
    setParams(p1,p2);
}

Mobius::~Mobius()
{
    delete[] points;
    delete[] normals;
}

void Mobius::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef(0.5,1,0.5);
    int i,j;
    for (i = 0; i < _p1; i++) {
        Vector3 ind1, ind1n, ind2, ind2n, ind3, ind3n, ind4, ind4n;
        for (j = 0; j < _p2-1; j++) {
            ind1 = points[i*_p2+j];
            ind1n = normals[j];
            ind2 = points[i*_p2+(j+1)];
            ind2n = normals[(j+1)];
            ind3 = points[(i+1)*_p2+j];
            ind3n = normals[j];
            ind4 = points[(i+1)*_p2+(j+1)];
            ind4n = normals[j+1];

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
            glBegin(GL_TRIANGLES);
            glNormal3f(-ind1n.x,-ind1n.y,-ind1n.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glNormal3f(-ind2n.x,-ind2n.y,-ind2n.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(-ind3n.x,-ind3n.y,-ind3n.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();
            glBegin(GL_TRIANGLES);
            glNormal3f(-ind2n.x,-ind2n.y,-ind2n.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(-ind4n.x,-ind4n.y,-ind4n.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glNormal3f(-ind3n.x,-ind3n.y,-ind3n.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();
        }
        // draw the glued edge
        // ie. reverse the triangulation
        j = _p2 - 1;
        ind1 = points[i*_p2+j];
        ind1n = normals[j];
        ind2 = points[(_p1-i)*_p2];
        ind2n = normals[0] * -1.0;
        ind3 = points[(i+1)*_p2+j];
        ind3n = normals[j];
        ind4 = points[(_p1-i-1)*_p2];
        ind4n = normals[0] * -1.0;
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
        glBegin(GL_TRIANGLES);
        glNormal3f(-ind1n.x,ind1n.y,ind1n.z);
        glVertex3f(ind1.x,ind1.y,ind1.z);
        glNormal3f(-ind2n.x,ind2n.y,ind2n.z);
        glVertex3f(ind2.x,ind2.y,ind2.z);
        glNormal3f(-ind3n.x,ind3n.y,ind3n.z);
        glVertex3f(ind3.x,ind3.y,ind3.z);
        glEnd();
        glBegin(GL_TRIANGLES);
        glNormal3f(-ind2n.x,-ind2n.y,-ind2n.z);
        glVertex3f(ind2.x,ind2.y,ind2.z);
        glNormal3f(-ind4n.x,-ind4n.y,-ind4n.z);
        glVertex3f(ind4.x,ind4.y,ind4.z);
        glNormal3f(-ind3n.x,-ind3n.y,-ind3n.z);
        glVertex3f(ind3.x,ind3.y,ind3.z);
        glEnd();
    }
    glPopMatrix();
}

void Mobius::build()
{
    if(_p2 < 3) {
        _p2 = 3;
    }

    delete[] points;
    delete[] normals;
    points = new Vector3[(_p1+1)*_p2];
    normals = new Vector3[_p2];
    int i,j;
    for (j = 0; j < _p2; j++) {
        float theta = M_PI*2*(j/(float)_p2);
        for (i = 0; i <= _p1; i++) {
            float v = 2*(i/(float)_p1)-1;
            float xz = (1.0+(1.0/2.0)*v*cos(theta/2.0));
            float x = xz*cos(theta);
            float y = (1.0/2.0)*v*sin(theta/2.0);
            float z = xz*sin(theta);
            Vector3 p = Vector3(x,y,z);
            points[i*_p2+j] = p;
            if(i == _p1/2) {
                normals[j] = p.cross(Vector3((1.0/2.0)*cos(theta/2.0)*cos(theta),
                                             y/v,
                                             (1.0/2.0)*cos(theta/2.0)*sin(theta)));
            }
        }
    }
}
