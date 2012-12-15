#ifndef CURVELOADER_H
#define CURVELOADER_H

#include <shape.h>
#include <jansson.h>

enum CurveType
{
    STRAIGHT_LINE,
    BEZIER,
    BSPLINE
};

class CurveLoader : public Shape
{
public:
    CurveLoader(int p, QString file);
    ~CurveLoader();
    void build();

    //! Loads the shape file from disk
    Vector3* loadPoints(json_t* points);
    void loadStraight(Vector3* points);
    void loadBezier(Vector3* points);
    void rotateCurve();

    //! Computes bezier curve based on index into array i
    void deCasteljau(int i, Vector3* seam, Vector3* curve, Vector3* normals);
protected:
    QString _file;
    CurveType _type;
    Vector3* _seam;
    int _seamSize;
};

#endif // CURVELOADER_H
