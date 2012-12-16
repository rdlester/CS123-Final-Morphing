#include "straightline.h"
#include "math/CS123Algebra.h"
#include "point.h"
#include "curve.h"

StraightLine::StraightLine() : Curve()
{
}

Vector2 normalize(Vector2 v)
{
    float mag = sqrt(v.x*v.x +v.y*v.y);
    return Vector2(v.x/mag, v.y/mag);
}

void StraightLine::addPoint(float x, float y){
    if (!isActive) {

        if(this->cPoints.size()>0)
        {
            Point last = cPoints.back();
            float dx = x -last.x;
            float dy = y - last.y;

            Vector2 normal = normalize(Vector2(dy, -dx));
            this->normals.push_back(normal);
        }

        cPoints.push_back(Point(x, y));
    }
}

void StraightLine::deleteActivePoint(){
    if (isActive) {
        if(activePointIndex == 0)
        {
            normals.pop_front();
        }
        else if(activePointIndex == cPoints.size()-1)
        {
            normals.pop_back();
        }
        else
        {
            float dx1 = cPoints[activePointIndex+1].x - cPoints[activePointIndex-1].x;
            float dy1 = cPoints[activePointIndex+1].y - cPoints[activePointIndex-1].y;

//            float mag = sqrt(dx1*dx1+dy1*dy1);
//            Vector2 normal = Vector2(dy1/mag, -dx1/mag);

            normals[activePointIndex-1] = normalize(Vector2(dy1, -dx1));;
            normals.remove(activePointIndex);
        }
        cPoints.remove(activePointIndex);
        isActive = false;
    }
}



void StraightLine::moveActivePoint(float dx, float dy){
    if(isActive){
        cPoints[activePointIndex].x = dx;
        cPoints[activePointIndex].y = dy;

        //update normal
            float x1,y1,x2,y2, mag;
            if(activePointIndex==0)
            {

                x2 = cPoints[activePointIndex+1].x;
                y2 = cPoints[activePointIndex+1].y;

                this->normals[activePointIndex] = normalize(Vector2(y2-dy, dx-x2));

            }else if (activePointIndex == cPoints.size()-1){

                x1 = cPoints[activePointIndex-1].x;
                y1 = cPoints[activePointIndex-1].y;

                this->normals[activePointIndex-1] = normalize(Vector2(dy-y1, x1-dx));
            }else{

                x2 = cPoints[activePointIndex+1].x;
                y2 = cPoints[activePointIndex+1].y;

                this->normals[activePointIndex] = normalize(Vector2(y2-dy, dx-x2));

                x1 = cPoints[activePointIndex-1].x;
                y1 = cPoints[activePointIndex-1].y;

                this->normals[activePointIndex-1] = normalize(Vector2(dy-y1, x1-dx));
            }
    }
}

void StraightLine::drawNormals()
{
    if(drawNormal && !cPoints.empty() && !normals.empty())
    {
        for(unsigned int i = 1; i<cPoints.size(); i++)
        {
                Vector2 n = normals[i-1];
                float x = (cPoints[i-1].x +cPoints[i].x)*0.5;
                float y = (cPoints[i-1].y +cPoints[i].y)*0.5;
                this->drawArrowLine(x,y,x+n.x*20,y+n.y*20);
        }
    }
}
