#include "cube.h"
Cube::Cube(int p1, int p2) : Shape()
{
    square = new Square(p1, p2);
    setParams(p1,p2);
}

Cube::~Cube()
{
    delete square;
}

void Cube::draw()
{
    /**
      * Draw faces one at a time
      * Use translations and rotations to position faces
      */
    glMatrixMode(GL_MODELVIEW);

    //Front
    glPushMatrix();
    glTranslatef(-0.5,-0.5,0.5);
    square->draw();
    glPopMatrix();

    //Back
    glPushMatrix();
    glTranslatef(0.5,-0.5,-0.5);
    glRotatef(180,0,1,0);
    square->draw();
    glPopMatrix();

    //Right
    glPushMatrix();
    glTranslatef(0.5,-0.5,0.5);
    glRotatef(90,0,1,0);
    square->draw();
    glPopMatrix();

    //Left
    glPushMatrix();
    glTranslatef(-0.5,-0.5,-0.5);
    glRotatef(270,0,1,0);
    square->draw();
    glPopMatrix();

    //Top
    glPushMatrix();
    glTranslatef(-0.5,0.5,0.5);
    glRotatef(270,1,0,0);
    square->draw();
    glPopMatrix();

    //Bottom
    glPushMatrix();
    glTranslatef(-0.5,-0.5,-0.5);
    glRotatef(90,1,0,0);
    square->draw();
    glPopMatrix();
}

void Cube::build()
{
    square->setParams(_p1,_p2);
}
