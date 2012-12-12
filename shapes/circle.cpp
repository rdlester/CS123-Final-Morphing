#include "circle.h"

Circle::Circle(int p1, int p2) : Shape()
{
    points = NULL;
    setParams(p1, p2);
}

Circle::~Circle()
{
    delete points;
}

void Circle::draw()
{
    // Start with inner circle
    glMatrixMode(GL_MODELVIEW);
    int i,j;
    Vector3 origin = points[0];
    Vector3 curr = points[1];
    for (j = 1; j <= _p2; j++) {
        Vector3 next = points[(j%_p2)+1];
        glBegin(GL_TRIANGLES);
        glNormal3f(0,0,1);
        glVertex3f(origin.x,origin.y,origin.z);
        glVertex3f(curr.x,curr.y,curr.z);
        glVertex3f(next.x,next.y,next.z);
        glEnd();
        curr = next;
    }

    // Now triangulate around and outward
    for (i = 0; i < _p1-1; i++){
        for (j = 0; j < _p2; j++){
            //1
            Vector3 ind1 = points[1 + i*_p2 + j];
            Vector3 ind2 = points[1 + (i+1)*_p2 + j];
            Vector3 ind3 = points[1 + (i+1)*_p2 + ((j+1)%_p2)];
            glBegin(GL_TRIANGLES);
            glNormal3f(0,0,1);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();

            //2
            Vector3 ind4 = points[1 + i*_p2 + ((j+1)%_p2)];
            glBegin(GL_TRIANGLES);
            glNormal3f(0,0,1);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glEnd();
        }
    }
}

void Circle::build()
{
    /**
      * Centered at origin w/ radius 0.5
      * p1 is radial subdivisions
      * p2 is angular subdivisions (min is 3)
      * flat on z-axis
      */
    // Clean up
    delete[] points;

    // Constrain _p2
    if(_p2 < 3) {
        _p2 = 3;
    }

    // Make the points
    points = new Vector3[_p1*_p2 + 1];

    points[0] = Vector3(0,0,0); // origin

    // Circle around, then step outwards and repeat
    int i, j;
    for (i = 0; i < _p1; i++) {
        float r = (i+1)/((float)_p1*2.0f);
        for (j = 0; j < _p2; j++) {
            float theta = M_PI * 2 * (j/(float)_p2);
            points[1 + i*_p2 + j] = Vector3(r*cos(theta),r*sin(theta),0);
        }
    }
}
