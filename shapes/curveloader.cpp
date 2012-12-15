#include "curveloader.h"

CurveLoader::CurveLoader(int p, QString file) : Shape()
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
    if(!curve) {
        std::cout << error.text << "\n";
    }

    CurveType _type = (CurveType)json_integer_value(json_object_get(curve, "type"));
    json_t* points = json_object_get(curve, "points");
    Vector3* loadedPoints = loadPoints(points);

    // switch on type:
    switch(_type) {
    case STRAIGHT_LINE:
        loadStraight(loadedPoints);
        break;
    case BEZIER:
        loadBezier(loadedPoints);
        break;
    }
}

Vector3* CurveLoader::loadPoints(json_t* points)
{
    // load the seam, then rotate
    _seamSize = json_array_size(points);
    Vector3* seam = new Vector3[_p];

    int i;
    for (i = 0; i < std::min(_seamSize,_p); i++) {
        json_t* point = json_array_get(points, i);
        double x = json_real_value(json_object_get(point,"x"));
        double y = json_real_value(json_object_get(point,"y"));
        seam[i] = Vector3(x,y,0);
    }
    for (i = 0; i < _p - _seamSize; i++) {
        seam[i+_seamSize] = seam[_seamSize-1];
    }

    _seamSize = std::min(_seamSize,_p);

    return seam;
}

void CurveLoader::loadStraight(Vector3* points)
{
    int i;
    for (i = 0; i < _p; i++) {
        _vertices[i] = points[i];
        // gotta compute normals!
        if (i == 0) {
            // do nothing
        } else {
            float dx = _vertices[i].x - _vertices[i-1].x;
            float dy = _vertices[i].y - _vertices[i-1].y;
            float norm = sqrt(dx*dx+dy*dy);
            if (i != 1) {
                _normals[i-1] = (Vector3(dx/norm,dy/norm,0)
                                 + _normals[i-1]).normalize();
            } else {
                _normals[i-1] = Vector3(dx/norm,dy/norm,0);
            }
        }
    }
    rotateCurve();
}

void CurveLoader::loadBezier(Vector3* points)
{
    int i;
    for (i = 0; i < _p; i++) {
        deCasteljau(i, points, _vertices, _normals);
    }

    // do the rotate
    rotateCurve();
}

void CurveLoader::deCasteljau(int i, Vector3* seam, Vector3* curve, Vector3* normals)
{
    float u = i/(float)(_p-1);

    Vector3 coef[_seamSize][_seamSize];

    for(int i=0; i < _p; i++){
        coef[_p-1][i] = seam[i];
    }

    for(int level = _p-2; level >= 0; level--)
    {
        for(int i =0 ; i <= level; i++)
        {
            float a = (1-u)*coef[level+1][i].x + u*coef[level+1][i+1].x;
            float b = (1-u)*coef[level+1][i].y + u*coef[level+1][i+1].y;

            coef[level][i] = Vector3(a,b,0);
        }
    }

    curve[i] = coef[0][0];

    //trying to compute the tangent here
    int dx = coef[1][1].x - coef[1][0].x;
    int dy = coef[1][1].y - coef[1][0].y;
    float mag = sqrt(dx*dx+dy*dy);

    normals[i] = Vector3(dy/mag, dx/mag, 0);
}

void CurveLoader::rotateCurve()
{

}
