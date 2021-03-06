#include "morpher.h"
#include "math.h"

Morpher::Morpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn, int p) : Shape(0)
{
    int pp = (int)pow(p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
    }
    setParams(p);
}
Morpher::Morpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn,
                 int p, QString path) : Shape(path)
{
    int pp = (int)pow(p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
    }
    setParams(p);
}
Morpher::Morpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn,
                 int p, int texId) : Shape(texId)
{
    int pp = (int)pow(p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
    }
    setParams(p);
}

Morpher::~Morpher()
{
    delete[] _shapeAv;
    delete[] _shapeAn;
    delete[] _shapeBv;
    delete[] _shapeBn;
}

void Morpher::setShapeA(Vector3* av, Vector3* an)
{
    delete[] _shapeAv;
    delete[] _shapeAn;

    int pp = (int)pow(_p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
    }
}

void Morpher::setShapeB(vec3<REAL> *bv, vec3<REAL> *bn)
{
    delete[] _shapeBv;
    delete[] _shapeBn;

    int pp = (int)pow(_p,2);
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
    }
}

void Morpher::build()
{
    // Initialize to shape A
    Shape::build();
    int pp = (int)pow(_p,2);
    for (int i = 0; i < pp; i++) {
        _vertices[i] = _shapeAv[i];
        _normals[i] = _shapeAn[i];
    }
}

void Morpher::morphTo(float t)
{
    // constrain t
    t = std::min(std::max(t,0.0f),1.0f);
    //t = t*t*(3.f - 2.f*t)

    Shape::build();

    // interpolate
    int pp = (int)pow(_p,2);
    int i;
    for (i = 0; i < pp; i++) {
        _vertices[i] = (1-t)*_shapeAv[i] + t*_shapeBv[i];
        _normals[i] = ((1-t)*_shapeAn[i] + t*_shapeBn[i]).getNormalized();
    }
}

void Morpher::lineMorph()
{
    Shape::build();

    // interpolate
    int pp = (int)pow(_p,2);
    int i;
    for (i = 0; i < pp; i++) {
        float t = (float)i/(float)(pp-1); // use index to determine blending
        _vertices[i] = (1-t)*_shapeAv[i] + t*_shapeBv[i];
        _normals[i] = ((1-t)*_shapeAn[i] + t*_shapeBn[i]).getNormalized();
    }
}

void Morpher::matrixMorph(float* alpha)
{
    Shape::build();

    int pp = (int)pow(_p,2);
    int i;
    for (i = 0; i < pp; i++) {
        float t = alpha[i];
        _vertices[i] = (1-t)*_shapeAv[i] + t*_shapeBv[i];
        _normals[i] = ((1-t)*_shapeAn[i] + t*_shapeBn[i]).getNormalized();
    }
}

void Morpher::randomMorph()
{
    int pp = (int)pow(_p,2);
    int i;
    for (i = 0; i < pp; i++) {
        float t = (float)rand() / RAND_MAX;
        _vertices[i] = (1-t)*_shapeAv[i] + t*_shapeBv[i];
        _normals[i] = ((1-t)*_shapeAn[i] + t*_shapeBn[i]).getNormalized();
    }
}
