#include "threemorpher.h"

ThreeMorpher::ThreeMorpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn,
                           Vector3* cv, Vector3* cn, int p) : Shape(0)
{
    int pp = (int)pow(p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    _shapeCv = new Vector3[pp];
    _shapeCn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
        _shapeCv[i] = cv[i];
        _shapeCn[i] = cn[i];
    }
    setParams(p);
}
ThreeMorpher::ThreeMorpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn,
                           Vector3* cv, Vector3* cn,
                           int p, QString path) : Shape(path)
{
    int pp = (int)pow(p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    _shapeCv = new Vector3[pp];
    _shapeCn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
        _shapeCv[i] = cv[i];
        _shapeCn[i] = cn[i];
    }
    setParams(p);
}
ThreeMorpher::ThreeMorpher(Vector3* av, Vector3* an, Vector3* bv, Vector3* bn,
                           Vector3* cv, Vector3* cn,
                           int p, int texId) : Shape(texId)
{
    int pp = (int)pow(p,2);
    _shapeAv = new Vector3[pp];
    _shapeAn = new Vector3[pp];
    _shapeBv = new Vector3[pp];
    _shapeBn = new Vector3[pp];
    _shapeCv = new Vector3[pp];
    _shapeCn = new Vector3[pp];
    int i;
    for (i = 0; i < pp; i++) {
        _shapeAv[i] = av[i];
        _shapeAn[i] = an[i];
        _shapeBv[i] = bv[i];
        _shapeBn[i] = bn[i];
        _shapeCv[i] = cv[i];
        _shapeCn[i] = cn[i];
    }
    setParams(p);
}

ThreeMorpher::~ThreeMorpher()
{
    delete[] _shapeAv;
    delete[] _shapeAn;
    delete[] _shapeBv;
    delete[] _shapeBn;
    delete[] _shapeCv;
    delete[] _shapeCn;
}

void ThreeMorpher::build()
{
    Shape::build();
    int pp = (int)pow(_p,2);
    for (int i = 0; i < pp; i++) {
        _vertices[i] = _shapeAv[i];
        _normals[i] = _shapeAn[i];
    }
}

void ThreeMorpher::morphTo(Vector3 t)
{
    int pp = (int)pow(_p,2);
    for (int i = 0; i < pp; i++) {
        _vertices[i] = t.x * _shapeAv[i] + t.y * _shapeBv[i] + t.z * _shapeCv[i];
        _normals[i] = (t.x * _shapeAn[i] + t.y * _shapeBn[i] + t.z * _shapeCn[i]).getNormalized();
    }
}

void ThreeMorpher::lineMorph(Vector3 from, Vector3 to)
{
    int pp = (int)pow(_p,2);
    for (int i = 0; i < pp; i++) {
        float metaT = (float)i/(float)(pp-1);
        Vector3 t = Vector3((1.f-metaT) * from.x + metaT * to.x,
                            (1.f-metaT) * from.y + metaT * to.y,
                            (1.f-metaT) * from.z + metaT * to.z);
        _vertices[i] = t.x * _shapeAv[i] + t.y * _shapeBv[i] + t.z * _shapeCv[i];
        _normals[i] = (t.x * _shapeAn[i] + t.y * _shapeBn[i] + t.z * _shapeCn[i]).getNormalized();
    }
}

void ThreeMorpher::matrixMorph(Vector3* alpha)
{
    int pp = (int)pow(_p,2);
    for (int i = 0; i < pp; i++) {
        Vector3 t = alpha[i];
        _vertices[i] = t.x * _shapeAv[i] + t.y * _shapeBv[i] + t.z * _shapeCv[i];
        _normals[i] = (t.x * _shapeAn[i] + t.y * _shapeBn[i] + t.z * _shapeCn[i]).getNormalized();
    }
}
