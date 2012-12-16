#include "bspline.h"

/*base code borrowed from UC Berkeley CS184 SP12 HW4 */

// The B-Spline drawing routine.
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {

    connectTheDots();
    /* YOUR CODE HERE */

    if(cPoints.size() > 1)
    {

        lPoints.clear();
        normals.clear();
        for(int i = 0; i<cPoints.size()-3; i++)
        {
            Point p0 = cPoints[i];
            Point p1 = cPoints[i+1];
            Point p2 = cPoints[i+2];
            Point p3 = cPoints[i+3];

            computeSegment(p0,p1,p2,p3, levelOfDetail);
        }

        for(int i=0; i<lPoints.size()-1; i++)
        {
            this->drawLine(lPoints[i].x, lPoints[i].y, lPoints[i+1].x, lPoints[i+1].y);
        }

        drawNormals();
    }
}


Point calculatePoint(float u, Point p0, Point p1, Point p2, Point p3)
{

    float uu = u*u;
    float uuu = u*u*u;

    float b0 = -uuu+3*uu-3u+1;
    //float b0 =1;
    float b1 = 3*uuu-6*uu+4;
    float b2 = -3*uuu+3*uu+3*u+1;
    float b3 = uuu;

    float x = b0*p0.x;
    x += b1*p1.x;
    x += b2*p2.x;
    x += b3*p3.x;

    float y = b0*p0.y;
    y += b1*p1.y;
    y += b2*p2.y;
    y += b3*p3.y;

    x = x/6.0f;
    y = y/6.0f;

    return Point(x, y);
}

//here we are using uniform cubic bspline
void Bspline::computeSegment(Point p1, Point p2, Point p3, Point p4, int levelOfDetail) {

    float x,y;

    float x1 = (-p1.x + 3 * p2.x - 3 * p3.x + p4.x) / 6.0;
    float x2  = (3 * p1.x - 6 * p2.x + 3 * p3.x) / 6.0;
    float x3  = (-3 * p1.x + 3 * p3.x) / 6.0;
    float x4  = (p1.x + 4 * p2.x + p3.x) / 6.0;
    float y1  = (-p1.y + 3 * p2.y - 3 * p3.y + p4.y) / 6.0;
    float y2  = (3 * p1.y - 6 * p2.y + 3 * p3.y) / 6.0;
    float y3 = (-3 * p1.y + 3 * p3.y) / 6.0;
    float y4 = (p1.y + 4 * p2.y + p3.y) / 6.0;

    for(int j = 0; j <= levelOfDetail; j++)
    {
        float u = j/(float)levelOfDetail;
        //Point p = calculatePoint(u, p1,p2,p3,p4);
        //qDebug()<<QString::number(normals.size());
        float uu = u*u;
        float uuu = u*u*u;

        x = uuu*x1 + uu*x2 + u*x3 +x4;
        y = uuu*y1 + uu*y2 + u*y3 +y4;

        if(!lPoints.empty())
        {
            Point last = lPoints.back();

            float dx = x - last.x;
            float dy = y - last.y;
            float mag = sqrt(dx*dx+dy*dy);
            Vector2 normal = Vector2(dy/mag, -dx/mag);

            if(mag<0.1) //if the segment is too small, use neighbor's normal
                normal = normals.back();

            normals.push_back(normal);
        }

        lPoints.push_back(Point(x,y));

    }

    //    Point p = Point(x,y);
    //    p.draw();
}


void Bspline::drawNormals()
{
    if(drawNormal && !normals.empty() && !lPoints.empty())
    {
        //qDebug()<<""<<lPoints.size();
        for(int i=0; i<lPoints.size()-1; i+=10)
        {
            Vector2 n = normals[i];
            float x = lPoints[i].x;
            float y = lPoints[i].y;
            this->drawArrowLine(x, y, x+n.x*20,y+n.y*20);
        }
    }
}
