#ifndef CURVE_H
#define CURVE_H

#include "point.h"
#include "QVector"
#include "math/CS123Algebra.h"
#include <QString>

/*This code is adapted from UC Berkeley CS184 SP12 HW4 support code*/

enum CurveType
{
    STRAIGHT_LINE,
    BEZIER,
    BSPLINE
};

class Curve
{
public:
    Curve();
    virtual ~Curve();
    virtual void draw(int levelOfDetail);
    virtual void moveActivePoint(float dx, float dy);

    virtual void addPoint(float x, float y);
    virtual void deleteActivePoint();
    virtual void updateActivePoint(float x, float y);
    virtual void drawNormals();

    //Point deCasteljau(float u, QVector<Point> points, QVector<Vector2>* normals);

    void connectTheDots();
    void drawLine(float x1, float y1, float x2, float y2) ;
    void drawArrowLine(float x1, float y1, float x2, float y2) ;
    void toggleNormal(int levelOfDetail);

    //! Saves control points + curve type to JSON
    void save(QString path);
    void setScreen(int width, int height);

    bool isActive;
    bool drawNormal;
    CurveType type;

    //protected:
    float _width, _height; // used for saving
    QVector<Point> cPoints; //  control points
    QVector<Point> lPoints; //  the points that are actually on the curve
    QVector<Vector2> normals;
    int activePointIndex;

};

#endif // CURVE_H
