#include "badcube.h"
BadCube::BadCube(int p) : Shape(0)
{
    setParams(p);
}

BadCube::BadCube(int p, QString path) : Shape(path)
{
    setParams(p);
}

BadCube::BadCube(int p, int texId) : Shape(_texId)
{
    setParams(p);
}

BadCube::~BadCube()
{
}

void BadCube::build()
{
    Shape::build();

    // note: might have to flip ordering of middle points! what a pain

    // a sixth of total points, used for drawing morphed circles
    // _p-1 so final point can be same as beginning
    int p6 = (_p-1)/6;

    // a half of total points, num of points not at pole or on middle seam
    // _p-3 to sub out N pole, S pole, and middle seam
    int p2 = (_p-3)/2;
    Vector3 normalpz, normalpy, normalmx, normalmz, normalmy, normalpx;

    int i, j;
    // Start with N point
    for (i = 0; i < _p; i++) {
        _vertices[i] = Vector3(0.5,0.5,-0.5);
        _normals[i] = (Vector3(1,1,-1)).getNormalized();
    }

    // tiny half-cubes on front faces
    normalpz = Vector3(1,0,0);
    normalpy = Vector3(1,0,0);
    normalmx = Vector3(0,1,0);
    normalmz = Vector3(0,1,0);
    normalmy = Vector3(0,0,-1);
    normalpx = Vector3(0,0,-1);
    for (i = 1; i < p2+1; i++) {
        float iinterp = (float)i/(float)(p2+1); // interp from 0 to 1
        // +z
        for (j = 0; j < p6; j++) {
            float jinterp = (float)j/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5,0.5-iinterp,-0.5+iinterp*jinterp);
            _normals[i*_p + j] = normalpz;
        }
        // +y
        for (j = p6; j < 2*p6; j++) {
            float jinterp = (float)(j-p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5,0.5-iinterp*(1.f-jinterp),-0.5+iinterp);
            _normals[i*_p + j] = normalpy;
        }
        // -x
        for (j = 2*p6; j < 3*p6; j++) {
            float jinterp = (float)(j-2*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp*jinterp,0.5,-0.5+iinterp);
            _normals[i*_p + j] = normalmx;
        }
        // -z
        for (j = 3*p6; j < 4*p6; j++) {
            float jinterp = (float)(j-3*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp,0.5,-0.5+iinterp*(1.f-jinterp));
            _normals[i*_p + j] = normalmz;
        }
        // -y
        for (j = 4*p6; j < 5*p6; j++) {
            float jinterp = (float)(j-3*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp,0.5-iinterp*jinterp,-0.5);
            _normals[i*_p + j] = normalmy;
        }
        // +x
        for (j = 5*p6; j < 6*p6; j++) {
            float jinterp = (float)(j-3*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp*(1.f-jinterp),-0.5-iinterp,-0.5);
            _normals[i*_p + j] = normalpx;
        }
        // last point is back to the start
        _vertices[i*_p + (_p-1)] = Vector3(0.5,-0.5-iinterp,-0.5);
        _normals[i*_p + (_p-1)] = normalpz;
    }

    // middle seam
    i = p2+1;
    normalpz = (Vector3(1,-1,0)).getNormalized();
    normalpy = (Vector3(1,0,1)).getNormalized();
    normalmx = (Vector3(0,1,1)).getNormalized();
    normalmz = (Vector3(-1,1,0)).getNormalized();
    normalmy = (Vector3(-1,0,-1)).getNormalized();
    normalpx = (Vector3(0,-1,-1)).getNormalized();
    // +z
    for (j = 0; j < p6; j++) {
        _vertices[i*_p + j] = Vector3(0.5,-0.5,-0.5+(float)j/(float)p6);
        _normals[i*_p + j] = normalpz;
    }
    // +y
    for (j = p6; j < 2*p6; j++) {
        _vertices[i*_p + j] = Vector3(0.5,-0.5+(float)j/(float)p6,0.5);
        _normals[i*_p + j] = normalpy;
    }
    // -x
    for (j = 2*p6; j < 3*p6; j++) {
        _vertices[i*_p + j] = Vector3(0.5-(float)j/(float)p6,0.5,0.5);
        _normals[i*_p + j] = normalmx;
    }
    // -z
    for (j = 3*p6; j < 4*p6; j++) {
        _vertices[i*_p + j] = Vector3(-0.5,0.5,0.5-(float)j/(float)p6);
        _normals[i*_p + j] = normalmz;
    }
    // -y
    for (j = 4*p6; j < 5*p6; j++) {
        _vertices[i*_p + j] = Vector3(-0.5,0.5-(float)j/(float)p6,-0.5);
        _normals[i*_p + j] = normalmy;
    }
    // +x
    for (j = 5*p6; j < 6*p6; j++) {
        _vertices[i*_p + j] = Vector3(-0.5+(float)j/(float)p6,-0.5,-0.5);
        _normals[i*_p + j] = normalpx;
    }
    // last point is back to the start
    _vertices[i*_p + (_p-1)] = Vector3(0.5,-0.5,-0.5);
    _normals[i*_p + (_p-1)] = normalpz;

    // tiny half-cubes on back faces
    normalpz = Vector3(0,-1,0);
    normalpy = Vector3(0,0,1);
    normalmx = Vector3(0,0,1);
    normalmz = Vector3(-1,0,0);
    normalmy = Vector3(-1,0,0);
    normalpx = Vector3(0,-1,0);
    for (i = p2+2; i < 2*p2+2; i++) {
        float iinterp = (float)(i-(p2+1))/(float)(p2+1); // interp from 1 to 0
        // +z
        for (j = 0; j < p6; j++) {
            float jinterp = (float)j/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp,-0.5,-0.5+iinterp+(1.f-iinterp)*jinterp);
            _normals[i*_p + j] = normalpz;
        }
        // +y
        for (j = p6; j < 2*p6; j++) {
            float jinterp = (float)(j-p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp,-0.5+(1.f-iinterp)*jinterp,0.5);
            _normals[i*_p + j] = normalpy;
        }
        // -x
        for (j = 2*p6; j < 3*p6; j++) {
            float jinterp = (float)(j-2*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(0.5-iinterp-(1.f-iinterp)*jinterp,-0.5+(1.f-iinterp),0.5);
            _normals[i*_p + j] = normalmx;
        }
        // -z
        for (j = 3*p6; j < 4*p6; j++) {
            float jinterp = (float)(j-3*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(-0.5,-0.5+(1.f-iinterp),0.5-(1.f-iinterp)*jinterp);
            _normals[i*_p + j] = normalmz;
        }
        // -y
        for (j = 4*p6; j < 5*p6; j++) {
            float jinterp = (float)(j-3*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(-0.5,-0.5+(1.f-iinterp)*(1.f-jinterp),-0.5+iinterp);
            _normals[i*_p + j] = normalmy;
        }
        // +x
        for (j = 5*p6; j < 6*p6; j++) {
            float jinterp = (float)(j-3*p6)/(float)p6;
            _vertices[i*_p + j] = Vector3(-0.5+(1.f-iinterp)*jinterp,-0.5,-0.5+iinterp);
            _normals[i*_p + j] = normalpx;
        }
        // last point is back to the start
        _vertices[i*_p + (_p-1)] = Vector3(0.5,-0.5-iinterp,-0.5);
        _normals[i*_p + (_p-1)] = normalpz;
    }

    // End with S point
    for (i = 0; i < _p; i++) {
        _vertices[(_p-1)*_p + i] = Vector3(-0.5,-0.5,0.5);
        _normals[(_p-1)*_p + i] = (Vector3(-1,-1,1)).getNormalized();
    }
}
