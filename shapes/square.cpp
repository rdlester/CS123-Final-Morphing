#include "square.h"

Square::Square(int p) : Shape(0)
{
    setParams(p);
}

Square::Square(int p, QString path) : Shape(path)
{
    setParams(p);
}

Square::Square(int p, int texId) : Shape(texId)
{
    setParams(p);
}

Square::~Square()
{
}


void Square::build()
{
    /**
      * x and y in [-0.5,0.5], z is 0
      * _p gives dims for width and height
      * that squared is total number of points.
      *
      * THIS IS THE SHAPE THAT EVERYTHING ELSE IS
      * DEFINED AGAINST.
      */
    // Make face
    // Starts at (0,0,0), increments first over rows then columns
    Shape::build();
    int i,j;
    for (i = 0; i < _p; i++) {
        for (j = 0; j < _p; j++) {
            _vertices[j + i*(_p)] = Vector3(i/(float)(_p-1) - 0.5,j/(float)(_p-1) - 0.5,0);
            _normals[j + i*(_p)] = Vector3(0,0,1);
        }
    }
}
