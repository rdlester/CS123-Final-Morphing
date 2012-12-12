#include "cylinder.h"

Cylinder::Cylinder(int p1, int p2) : Shape()
{
    circle = new Circle(p1,p2);
    points = NULL;
    normals = NULL;
    setParams(p1, p2);
}

Cylinder::~Cylinder()
{
    delete circle;
    delete[] points;
    delete[] normals;
}

void Cylinder::draw()
{
    glMatrixMode(GL_MODELVIEW);

    // Base
    glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(0,0,0.5);
    circle->draw();
    glPopMatrix();

    // Top
    glPushMatrix();
    glRotatef(270,1,0,0);
    glTranslatef(0,0,0.5);
    circle->draw();
    glPopMatrix();

    // Sides
    glPushMatrix();
    glTranslatef(0,-0.5,0);

    int j,i;
    for (j = 0; j < _p2; j++) {
        for (i = 0; i < _p1; i++) {
            Vector3 ind1 = points[j*(_p1+1)+i];
            Vector3 ind2 = points[j*(_p1+1)+i+1];
            Vector3 ind3 = points[((j+1)%_p2)*(_p1+1)+i];
            Vector3 norm1 = normals[j];
            Vector3 norm2 = normals[(j+1)%_p2];

            // 1
            glBegin(GL_TRIANGLES);
            glNormal3f(norm1.x,norm1.y,norm1.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glNormal3f(norm1.x,norm1.y,norm1.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(norm2.x,norm2.y,norm2.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();

            // 2
            Vector3 ind4 = points[((j+1)%_p2)*(_p1+1)+i+1];
            glBegin(GL_TRIANGLES);
            glNormal3f(norm2.x,norm2.y,norm2.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glNormal3f(norm1.x,norm1.y,norm1.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(norm2.x,norm2.y,norm2.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glEnd();
        }
    }

    glPopMatrix();
}

void Cylinder::build()
{
    /**
      * Build cylinder edge centered at (0,0,0)
      * y in [0,1], x+z angular
      */
    if(_p2 < 3) {
        _p2 = 3;
    }
    circle->setParams(_p1,_p2);

    delete[] points;
    delete[] normals;
    points = new Vector3[(_p1+1)*_p2];
    normals = new Vector3[_p2];
    int i,j;
    float r = 0.5;
    for (j = 0; j < _p2; j++) { // Order is down then around
        // this way we only have to calculate each theta once
        float theta = M_PI * 2 * (j/(float)_p2);
        float x = r*cos(theta);
        float z = r*sin(theta);
        normals[j] = Vector3(x/r, 0, z/r);
        for (i = 0; i <= _p1; i++) { // scale the edge
            points[i + j*(_p1+1)] = Vector3(x,i/(float)_p1,z);
        }
    }
}
