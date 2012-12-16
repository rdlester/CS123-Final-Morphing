#include "cube.h"

Cube::Cube()
{
}

Cube::~Cube()
{

}

void Cube::build()
{
    int i,j;
    // N face
    for (j = 0; j < _p; j++) {
        _vertices[j] = Vector3(0,0.5,0);
        _normals[j] = Vector3(0,1,0);
    }

    // S face
    for (j = 0; j < _p; j++) {
        _vertices[(_p-1)*_p + j] = Vector3(0,0.5,0);
        _normals[j] = Vector3(0,1,0);
    }
}
