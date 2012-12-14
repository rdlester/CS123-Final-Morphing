#include "shape.h"

Shape::Shape()
{
    _vertices = NULL;
    _normals = NULL;
}

Shape::~Shape()
{
    delete[] _vertices;
    delete[] _normals;
}

void Shape::setParams(int p)
{
    if(_p != p) {
        _p = p;
        build();
    }
}

void Shape::build()
{
    delete[] _vertices;
    delete[] _normals;
    int pp = (int)pow(_p,2);
    _vertices = new Vector3[pp];
    _normals = new Vector3[pp];
}

void Shape::draw()
{
    /**
      * Draw faces in two parts
      */
    glMatrixMode(GL_MODELVIEW);
    int i,j;
    for (i = 0; i < (_p-1); i++) {
        for (j = 0; j < (_p-1); j++) {
            // 1
            Vector3 ind1 = _vertices[i*(_p)+j];
            Vector3 n1 = _normals[i*(_p)+j];
            Vector3 ind2 = _vertices[(i+1)*(_p)+j];
            Vector3 n2 = _normals[(i+1)*(_p)+j];
            Vector3 ind3 = _vertices[i*(_p)+j+1];
            Vector3 n3 = _normals[i*(_p)+j+1];

            glBegin(GL_TRIANGLES);
            glNormal3f(n1.x,n1.y,n1.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(n3.x,n3.y,n3.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();

            // 2
            Vector3 ind4 = _vertices[(i+1)*(_p)+j+1];
            Vector3 n4 = _normals[(i+1)*(_p)+j+1];
            glBegin(GL_TRIANGLES);
            glNormal3f(n3.x,n3.y,n3.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);
            glNormal3f(n4.x,n4.y,n4.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glEnd();
        }
    }
}

Vector3* Shape::getVertices()
{
    return _vertices;
}

Vector3* Shape::getNormals()
{
    return _normals;
}

int Shape::getNumVertices()
{
    return pow(_p, 2);
}
