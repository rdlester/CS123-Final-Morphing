#include "square.h"

Square::Square(int p1, int p2) : Shape()
{
    points = NULL;
    setParams(p1, p2);
}

Square::~Square()
{
    delete[] points;
}

void Square::draw()
{
    /**
      * Draw faces in two parts
      */
    glMatrixMode(GL_MODELVIEW);
    int i,j;
    for (i = 0; i < _p1; i++) {
        for (j = 0; j < _p1; j++) {
            // 1
            Vector3 ind1 = points[i*(_p1+1)+j];
            Vector3 ind2 = points[(i+1)*(_p1+1)+j];
            Vector3 ind3 = points[i*(_p1+1)+j+1];
            glBegin(GL_TRIANGLES);
            glNormal3f(0,0,1);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();

            // 2
            Vector3 ind4 = points[(i+1)*(_p1+1)+j+1];
            glBegin(GL_TRIANGLES);
            glNormal3f(0,0,1);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glEnd();
        }
    }
}

void Square::build()
{
    /**
      * x and y in [0,1], z is 0
      * _p1 gives # of squares along edge
      * _p1 + 1 gives # of points along edge
      * that squared is total number of points
      */

    // Start by clearing memory
    delete[] points;

    // Make face
    // Starts at (0,0,0), increments first over rows then columns
    points = new Vector3[(int)pow(_p1+1,2)];
    int i,j;
    for (i = 0; i < _p1+1; i++) {
        for (j = 0; j < _p1+1; j++) {
            points[j + i*(_p1+1)] = Vector3(i/(float)_p1,j/(float)_p1,0);
        }
    }
}
