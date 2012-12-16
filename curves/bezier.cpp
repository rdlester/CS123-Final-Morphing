#include "bezier.h"
#include "point.h"
#include "math/CS123Algebra.h"
#include "QDebug"

/*base code borrowed from UC Berkeley CS184 SP12 HW4 */

void Bezier::draw(int levelOfDetail) {

    connectTheDots();
    if(cPoints.size() > 1)
    {

        lPoints.clear();
        normals.clear();
        for(int j = 0; j <= levelOfDetail; j++)
        {
            float t = j/(float)levelOfDetail;
            Point p = deCasteljau(t, cPoints, &normals);

            //qDebug()<<QString::number(normals.size());
            lPoints.push_back(p);
        }

        for(int i=0; i<lPoints.size()-1; i++)
        {
            this->drawLine(lPoints[i].x, lPoints[i].y, lPoints[i+1].x, lPoints[i+1].y);
        }
        drawNormals();

    }

}

void Bezier::drawNormals()
{
    if(drawNormal && !normals.empty() && !lPoints.empty())
    {
        //qDebug()<<""<<lPoints.size();
        for(int i=0; i<lPoints.size()-1; i+=2)
        {
            Vector2 n = normals[i];
            float x = lPoints[i].x;
            float y = lPoints[i].y;
            this->drawArrowLine(x, y, x+n.x*20,y+n.y*20);
        }
    }
}

Point Bezier::deCasteljau(float u, QVector<Point> points, QVector<Vector2>* normals)
{

    const int n = points.size()-1;

    Point ps[n+1][n+1];

    for(int level= n; level>=0; level--)
    {
        if(level==n)
        {
            for(int i=0; i<=n; i++){
                ps[level][i] = points[i];
            }
            continue;
        }
        for(int i =0 ; i <= level; i++)
        {
            float a = (1-u)*ps[level+1][i].x + u*ps[level+1][i+1].x;
            float b = (1-u)*ps[level+1][i].y + u*ps[level+1][i+1].y;

            ps[level][i] = Point(a,b);
        }
    }

    //trying to compute the tangent here
    float dx = ps[1][1].x - ps[1][0].x;
    float dy = ps[1][1].y - ps[1][0].y;
    float mag = sqrt(dx*dx+dy*dy);
    Vector2 normal = Vector2(dy/mag, -dx/mag);

    normals->push_back(normal);

    Point toReturn = ps[0][0];

    //this->drawLine(ps[1][1].x,ps[1][1].y, ps[1][0].x,ps[1][0].y);
    //this->drawLine(toReturn.x, toReturn.y, toReturn.x+normal.x, toReturn.y+normal.y);
    return toReturn;
}


//Point calculateBezierPoint(float t, Point p0, Point p1, Point p2, Point p3)
//{
//    float u = 1-t;
//    float tt = t*t;
//    float uu = u*u;
//    float uuu = uu*u;
//    float ttt = tt*t;

//    float x = uuu*p0.x;
//    x += 3*uu*t*p1.x;
//    x += 3*u*tt*p2.x;
//    x += ttt*p3.x;

//    float y = uuu*p0.y;
//    y += 3*uu*t*p1.y;
//    y += 3*u*tt*p2.y;
//    y += ttt*p3.y;

//    return Point(x, y);
//}
