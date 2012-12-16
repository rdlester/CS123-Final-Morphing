#include "curve.h"
#include "point.h"
#include "vector"
#include "qgl.h"
#include "QDebug"
#include "bezier.h"
#include "math/CS123Algebra.h"
#include "jansson.h"

Curve::Curve()
{
    isActive = false;
    drawNormal = false;
}

Curve::~Curve()
{

}

void Curve::connectTheDots(){

    if(cPoints.size() > 1)
    {
        for(int i = 0; i<cPoints.size()-1; i++)
        {
            glColor3f(0, 1, 0);
            glLineWidth(3.0) ;
            glBegin( GL_LINES );
            glVertex2f(cPoints[i].x, cPoints[i].y);
            glVertex2f(cPoints[i+1].x, cPoints[i+1].y);
            glEnd();
        }
    }

    for(int i = 0; i<cPoints.size(); i++)
    {
        cPoints[i].draw();
    }

}

void Curve::draw(int levelOfDetail){
    if (!cPoints.empty()) {
        connectTheDots();
        drawNormals();
    }
}

void Curve::addPoint(float x, float y){
    if (!isActive) {
        cPoints.push_back(Point(x, y));
    }
}

void Curve::deleteActivePoint(){
    if (isActive) {

        cPoints.remove(activePointIndex);

        isActive = false;
    }
}

void Curve::moveActivePoint(float dx, float dy){
    if(isActive){
        cPoints[activePointIndex].x = dx;
        cPoints[activePointIndex].y = dy;
    }
}

void Curve::updateActivePoint(float x, float y){
    float dx, dy, dist2;
    float radius = Point::ACTIVE_RADIUS*Point::ACTIVE_RADIUS;

    dist2 = 100000;

    if (isActive) {
        dx = cPoints[activePointIndex].x - x;
        dy = cPoints[activePointIndex].y - y;
        dist2 = dx*dx + dy*dy;
    }

    if (dist2 > radius) {
        if (isActive)
        {
            cPoints[activePointIndex].active = false;
            isActive = false ;
        }
        for(int i = 0; i<cPoints.size(); i++)
        {
            dx = cPoints[i].x - x;
            dy = cPoints[i].y - y;
            dist2 = dx*dx + dy*dy;
            if (dist2 < radius) {
                activePointIndex = i;
                //qDebug()<<"Active point:"<<QString::number(i);
                cPoints[i].active = true;
                isActive = true ;
                break;
            }
        }
    }
}

void Curve::drawLine(float x1, float y1, float x2, float y2){

    glColor3f(0,0,1);
    glBegin( GL_LINES );
    glVertex2f( x1, y1 );
    glVertex2f( x2, y2 );
    glEnd();
}

void Curve::drawArrowLine(float x1, float y1, float x2, float y2){

    float dx = x2 - x1;
    float dy = y2 - y1;

    glColor3f(1,0,0);

    glBegin( GL_LINES );
    glVertex2f( x1, y1 );
    glVertex2f( x2, y2 );
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f( x2+0.5*dx, y2+0.5*dy);
    glVertex2f( x2-0.3*dy, y2+0.3*dx);
    glVertex2f( x2+0.3*dy, y2-0.3*dx);
    glEnd();
}


void Curve::drawNormals()
{

}


void Curve::toggleNormal(int levelOfDetail)
{
    if(drawNormal)
    {
        drawNormal = false;
    }
    else
    {
        drawNormal = true;
    }

    this->draw(levelOfDetail);
}

void Curve::setScreen(int width, int height)
{
    _width = (float)width;
    _height = (float)height;
}

void Curve::save(QString path)
{
    // Object format:
    // 'type': int from enum CurveType
    // 'points': array of point objects (x,y values)
    json_t *root = json_object();

    // Type
    json_object_set_new(root, "type", json_integer(type));

    // fill this array with points!
    json_t *points = json_array();
    for (unsigned int i = 0; i < cPoints.size(); i++) {
        // unpack control point and map coords to x in [0,1], y in [-0.5,0.5]
        // x in different region to make sure rotation around y-axis works properly
        Point currPt = cPoints.at(i);
        json_t *x = json_real(currPt.x/(_width));
        json_t *y = json_real(0.5 - currPt.y/_height);

        // pack into json object
        json_t *vec = json_object();
        json_object_set_new(vec, "x", x);
        json_object_set_new(vec, "y", y);

        // append to root array
        json_array_append_new(points, vec);
    }
    // add points to the object
    json_object_set_new(root,"points",points);

    // encode and save
    json_dump_file(root, path.toAscii(), JSON_PRESERVE_ORDER);

    // delete json
    json_decref(root);
}

