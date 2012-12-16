#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <square.h>
#include <sphere.h>
#include <circle.h>
#include <cylinder.h>
#include <cone.h>
#include <badcube.h>
#include <torus.h>
#include <morpher.h>
#include <threemorpher.h>
#include <curveloader.h>
#include <camera.h>

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime time;
    QTimer timer;

    void initializeGL();
    void setLights();

    void paintGL();
    void applyPerspectiveCamera(float width, float height);

    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    OrbitCamera m_camera;
    GLUquadric* m_quadric;
    Vector2 m_prevMousePos;

    int _p;
    Morpher* _morph;
    ThreeMorpher* _3morph;
    Square* _square;
    BadCube* _badcube;
    Sphere* _sphere;
    Circle* _circle;
    Cylinder* _cylinder;
    Cone* _cone;
    CurveLoader* _curve1;
    double _t;
    double _step;
    bool _dir;
    float _tick;
    float* _alpha;

private slots:
    void tick();
};

#endif // VIEW_H

