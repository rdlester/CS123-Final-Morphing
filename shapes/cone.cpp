#include "cone.h"

Cone::Cone(int p1, int p2) : Shape()
{
    circle = new Circle(p1,p2);
    points = NULL;
    normals = NULL;
    setParams(p1, p2);
}

Cone::~Cone()
{
    delete circle;
    delete[] points;
    delete[] normals;
}

void Cone::draw()
{
    glMatrixMode(GL_MODELVIEW);

    // Base
    glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(0,0,0.5);
    circle->draw();
    glPopMatrix();

    // Cone
    glPushMatrix();
    glTranslatef(0,-0.5,0);

    int i,j;
    Vector3 tip = points[_p1*_p2];
    for (j = 0; j < _p2; j++) {
        for (i = 0; i < _p1; i++) {
            Vector3 ind1 = points[j*_p1+i];
            Vector3 norm1 = normals[j];
            Vector3 ind2 = points[((j+1)%_p2)*_p1+i];
            Vector3 norm2 = normals[(j+1)%_p2];
            Vector3 ind3, norm3, ind4, norm4;
            if(i < _p1-1) {
                ind3 = points[((j+1)%_p2)*_p1+i+1];
                norm3 = norm2;
                ind4 = points[j*_p1+i+1];
                norm4 = norm1;
            } else {
                ind3 = tip;
                norm3 = normals[j+_p2];
                ind4 = Vector3(INFINITY,INFINITY,INFINITY);
                norm4 = Vector3(INFINITY,INFINITY,INFINITY);
            }


            // 1
            glBegin(GL_TRIANGLES);
            glNormal3f(norm1.x,norm1.y,norm1.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glNormal3f(norm3.x,norm3.y,norm3.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glNormal3f(norm2.x,norm2.y,norm2.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glEnd();

            if(i < _p1-1) { // don't draw again if it's the tip
                // 2
                glBegin(GL_TRIANGLES);
                glNormal3f(norm1.x,norm1.y,norm1.z);
                glVertex3f(ind1.x,ind1.y,ind1.z);
                glNormal3f(norm4.x,norm4.y,norm4.z);
                glVertex3f(ind4.x,ind4.y,ind4.z);
                glNormal3f(norm3.x,norm3.y,norm3.z);
                glVertex3f(ind3.x,ind3.y,ind3.z);
                glEnd();
            }
        }
    }

    glPopMatrix();
}

void Cone::build()
{
    /**
      * Build cone w/ tip at (0,1,0)
      *
      */

    if(_p2 < 3) {
        _p2 = 3;
    }
    circle->setParams(_p1,_p2);

    // Make cone
    delete[] points;
    delete[] normals;
    points = new Vector3[_p1*_p2+1];
    normals = new Vector3[2*_p2];

    int i,j;
    float r = 0.5;
    double ynorm = 1/sqrt(5);
    double rnorm = 2/sqrt(5);
    for (j = 0; j < _p2; j++) {
        float theta = M_PI * 2 * (j/(float)_p2);
        float x = r*cos(theta);
        float z = r*sin(theta);
        normals[j] = Vector3(rnorm*x/r,ynorm,rnorm*z/r);
        for (i = 0; i < _p1; i++) {
            points[i+j*_p1] = Vector3((_p1-i)*x/(float)_p1,i/(float)_p1,(_p1-i)*z/(float)_p1);
        }
    }

    // tip and its normals
    points[_p1*_p2] = Vector3(0,1,0);
    for(j = 0; j < _p2; j++) {
        float theta = M_PI * 2 * (2*j+1)/(float)(2*_p2);
        normals[j+_p2] = Vector3(rnorm*cos(theta),ynorm,rnorm*sin(theta));
    }
}
