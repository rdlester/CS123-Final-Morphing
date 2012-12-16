#include "curveloader.h"

CurveLoader::CurveLoader(int p, QString file) : Shape(0)
{
    _file = file;
    setParams(p);
}
CurveLoader::CurveLoader(int p, QString file, QString path) : Shape(path)
{
    _file = file;
    setParams(p);
}
CurveLoader::CurveLoader(int p, QString file, int texId) : Shape(texId)
{
    _file = file;
    setParams(p);
}

CurveLoader::~CurveLoader()
{

}

void CurveLoader::build()
{
    Shape::build();

    // Load json from disk
    json_error_t error;
    json_t* curve = json_load_file(_file.toAscii(), 0, &error);
    if(!curve) { // if curve can't load break building
        std::cout << error.text << "\n";
        return;
    }

    // load type and points from json
    _type = (CurveType)json_integer_value(json_object_get(curve, "type"));
    json_t* points = json_object_get(curve, "points");
    Vector3* loadedPoints = loadPoints(points);

    // delete json object
    json_decref(curve);

    // build seam
    switch(_type) {
    case STRAIGHT_LINE:
        loadStraight(loadedPoints);
        break;
    case BEZIER:
        loadBezier(loadedPoints);
        break;
    case BSPLINE:
        loadBspline(loadedPoints);
        break;
    }
    delete[] loadedPoints;

    // rotate it to complete shape
    rotateCurve();
}

Vector3* CurveLoader::loadPoints(json_t* points)
{
    // load the seam, then rotate
    _seamSize = json_array_size(points);
    Vector3* seam = new Vector3[_seamSize];

    int i;
    for (i = 0; i < _seamSize; i++) {
        json_t* point = json_array_get(points, i);
        double x = json_real_value(json_object_get(point,"x"));
        double y = json_real_value(json_object_get(point,"y"));
        seam[i] = Vector3(x,y,0);
    }

    _seamSize = std::min(_seamSize,_p);

    return seam;
}

void CurveLoader::loadStraight(Vector3* points)
{
    int i;
    // copy over seam and compute normals
    for (i = 0; i < _seamSize; i++) {
        _vertices[i] = points[i];

        // compute normals
        if (i == 0) {
            _normals[i] = Vector3(0,0,0);
        } else {
            Vector3 thisV = _vertices[i];
            Vector3 lastV = _vertices[i-1];
            float dx = _vertices[i].x - _vertices[i-1].x;
            float dy = _vertices[i].y - _vertices[i-1].y;
            float norm = sqrt(dx*dx+dy*dy);
            dx /= norm;
            dy /= norm;
            Vector3 n = Vector3(-dy,dx,0);
            _normals[i-1] = (n+_normals[i-1]).getNormalized();
            _normals[i] = n;
        }
    }
    for (i = _seamSize; i < _p; i++) {
        _vertices[i] = _vertices[_seamSize-1];
        _normals[i] = _normals[_seamSize-1];
    }
}

void CurveLoader::loadBezier(Vector3* points)
{
    int i;
    for (i = 0; i < _p; i++) {
        deCasteljau(i, points);
    }
}

void CurveLoader::deCasteljau(int i, Vector3* seam)
{
    // convert index into interpolation amount
    float u = i/(float)(_p-1);

    // bezier basis coeficients
    Vector3 coef[_seamSize][_seamSize];

    // initialize with control points
    for(int i=0; i < _seamSize; i++){
        coef[_seamSize-1][i] = seam[i];
    }

    for(int level = _seamSize-2; level >= 0; level--) {
        for(int i =0; i <= level; i++) {
            // interpolate points from previous level
            float a = (1-u)*coef[level+1][i].x + u*coef[level+1][i+1].x;
            float b = (1-u)*coef[level+1][i].y + u*coef[level+1][i+1].y;

            coef[level][i] = Vector3(a,b,0);
        }
    }

    // final result is at [0][0]
    _vertices[i] = coef[0][0];

    // compute the normal using previous levels
    Vector3 c1 = coef[1][1];
    Vector3 c2 = coef[1][0];
    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float mag = sqrt(dx*dx+dy*dy);
    _normals[i] = Vector3(-dy/mag, dx/mag, 0);
}

void CurveLoader::loadBspline(vec3<REAL> *points)
{

}

void CurveLoader::rotateCurve()
{
    // we've filled up first _p entries of curve
    // create the rest by rotating
    Matrix4x4 rotateStep = getRotYMat(-2.0*M_PI/(_p-1)); // _p-1 b/c last step should be repeat of seam

    int i,j;
    for (i = 1; i < _p; i++) {
        for (j = 0; j < _p; j++) {
            // take previous step and rotate it
            _vertices[i*_p+j] = rotateStep * _vertices[(i-1)*_p+j];
            _normals[i*_p+j] = rotateStep * _normals[(i-1)*_p+j];
        }
    }
}
