#include "shape.h"
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QGLWidget>

Shape::Shape(QString texturePath)
{
    _texId = loadTexture(texturePath);
    if (_texId <= 0) {
        qDebug() << "Texture failed to load!: " << texturePath << "\n";
    }
    _vertices = NULL;
    _normals = NULL;
}

Shape::Shape(int texId) : _texId(texId)
{
    _vertices = NULL;
    _normals = NULL;
}

Shape::~Shape()
{
    if (glIsTexture(_texId)) {
        GLuint ti = (GLuint)_texId;
        glDeleteTextures(1,&ti);
    }
    delete[] _vertices;
    delete[] _normals;
}

int Shape::loadTexture(QString texturePath)
{
    // Make sure the image file exists
    QFile file(texturePath);
    if (!file.exists()) {
        return -1;
    }

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Create and load new texture
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(),
                      GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    return id;
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
    // load texture if set
    if (glIsTexture(_texId)) {
        glBindTexture(GL_TEXTURE_2D, _texId);
    }
    /**
      * Draw faces in two parts
      */
    int i,j;
    for (i = 0; i < (_p-1); i++) {
        double s1 = (double)i/(double)(_p-1);
        double s2 = (double)(i+1)/(double)(_p-1);
        for (j = 0; j < (_p-1); j++) {
            double t1 = (double)(j)/(double)(_p-1);
            double t2 = (double)(j+1)/(double)(_p-1);
            // 1
            Vector3 ind1 = _vertices[i*(_p)+j];
            Vector3 n1 = _normals[i*(_p)+j];
            Vector3 ind2 = _vertices[(i+1)*(_p)+j];
            Vector3 n2 = _normals[(i+1)*(_p)+j];
            Vector3 ind3 = _vertices[i*(_p)+j+1];
            Vector3 n3 = _normals[i*(_p)+j+1];

            glBegin(GL_TRIANGLES);
//            glColor3f(1.f,1.f,1.f);

            glTexCoord2f(s1,t1);
            glNormal3f(n1.x,n1.y,n1.z);
            glVertex3f(ind1.x,ind1.y,ind1.z);

            glTexCoord2d(s2,t1);
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);

            glTexCoord2d(s1,t2);
            glNormal3f(n3.x,n3.y,n3.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);
            glEnd();

            // 2
            Vector3 ind4 = _vertices[(i+1)*(_p)+j+1];
            Vector3 n4 = _normals[(i+1)*(_p)+j+1];
            glBegin(GL_TRIANGLES);
//            glColor3f(1.f,1.f,1.f);

            glTexCoord2d(s1,t2);
            glNormal3f(n3.x,n3.y,n3.z);
            glVertex3f(ind3.x,ind3.y,ind3.z);

            glTexCoord2d(s2,t1);
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3f(ind2.x,ind2.y,ind2.z);

            glTexCoord2d(s2,t2);
            glNormal3f(n4.x,n4.y,n4.z);
            glVertex3f(ind4.x,ind4.y,ind4.z);
            glEnd();
        }
    }
}

void Shape::drawNormals()
{
    int pp = (int)pow(_p,2);
    int i;
    float scale = 0.5;
    for (i = 0; i < pp; i++) {
        Vector3 n = _normals[i];
        Vector3 v = _vertices[i];
        Vector3 nv = Vector3(v.x+scale*n.x, v.y+scale*n.y, v.z+scale*n.z);

        // line out
        glBegin(GL_LINES);
        glColor3f(1.0f,1.f,1.f);
        glVertex3f(v.x,v.y,v.z);
        glVertex3f(nv.x, nv.y, nv.z);
        glEnd();
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

int Shape::getTexId()
{
    return _texId;
}
