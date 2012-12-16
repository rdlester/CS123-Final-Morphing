#include "view.h"
#include <QApplication>
#include <QKeyEvent>

View::View(QWidget *parent) : QGLWidget(parent)
{
    // View needs all mouse move events, not just mouse drag events
    //setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    //setCursor(Qt::BlankCursor);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

    m_camera.center = Vector3(0.f, 0.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 0.5f, m_camera.phi = 0.2f;
    m_camera.fovy = 60.f;

    m_quadric = gluNewQuadric();

    _p = 25;
    _square = new Square(_p);
    _badcube = new BadCube(_p);
    _sphere = new Sphere(_p);
    _circle = new Circle(_p);
    _cylinder = new Cylinder(_p);
    _cone = new Cone(_p);
    QString path = "/Users/Ryan/Documents/Brown/Masters1/GFX/final/newCurve.js";
    _curve1 = new CurveLoader(_p, path);
    _morph = new Morpher(_sphere->getVertices(), _sphere->getNormals(),
                         _curve1->getVertices(), _curve1->getNormals(),
                         _p);
//    _morph->morphTo(0.0f);
    _morph->lineMorph();
    _3morph = new ThreeMorpher(_sphere->getVertices(), _sphere->getNormals(),
                               _curve1->getVertices(), _curve1->getNormals(),
                               _cone->getVertices(), _cone->getNormals(),
                               _p);
//    _3morph->morphTo(Vector3(0.15,0.6,0.25));
    _3morph->lineMorph(Vector3(0.5,0.5,0), Vector3(0,0.25,0.75));
    _t = 0.0;
    _step = 0.05;
    _dir = true;
    _tick = 0;
    int pp = (int)pow(_p,2);
    _alpha = new float[pp];
    for (int i = 0; i < pp; i++) {
        _alpha[i] = 0.f;
    }
}

View::~View()
{
    delete _morph;
    delete _3morph;
    delete _square;
    delete _badcube;
    delete _sphere;
    delete _circle;
    delete _cylinder;
    delete _cone;
    delete _curve1;
    delete _alpha;
}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // set up canvas
    glViewport(0, 0, width(), height());

    applyPerspectiveCamera(this->width(),this->height());

    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);
//    _square->draw();
    //    _square->drawNormals();
//        _badcube->draw();
//    _sphere->draw();
    //    _circle->draw();
    //    _cylinder->draw();
    //    _cone->draw();
    //    _cone->drawNormals();
    //    _curve1->draw();
    //    _curve1->drawNormals();
    _morph->draw();
    //    _morph->drawNormals();
//    _3morph->draw();
//        _3morph->drawNormals();

    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_FILL);
//    _square->draw();
//    _square->drawNormals();
//    _badcube->draw();
//    _sphere->draw();
//    _circle->draw();
//    _cylinder->draw();
//    _cone->draw();
//    _cone->drawNormals();
//    _curve1->draw();
//    _curve1->drawNormals();
    _morph->draw();
//    _morph->drawNormals();
//    _3morph->draw();
//    _3morph->drawNormals();


}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    //time.start();
    timer.start(17);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Lighting (disable colormaterial when wireframes is enabled
    glEnable(GL_LIGHTING);
    setLights();

//    glEnable(GL_COLOR_MATERIAL);

    // for drawing lines
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // hide backfaces
//    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // wireframes, no faces
//    glColor3f(0, 0, 0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    //QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,2.0,0.0);


}

void View::setLights()
{
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Vector4(0.25, 0.5, 0.5, 0).data);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Vector4(0.75, 0.5, 0.5, 0).data);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Vector4(1, 1, 1, 0).data);
    glLightfv(GL_LIGHT0, GL_POSITION, Vector4(1, 2, 3, 0).getNormalized().data);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4(1, 1, 1, 0).data);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
}

/**
  Borrowed from lab9
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
void View::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    Vector3 dir(-Vector3::fromAngles(m_camera.theta, m_camera.phi));
    Vector3 eye(m_camera.center - dir * m_camera.zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, ratio, 0.1f, 1000.f);
    gluLookAt(eye.x, eye.y, eye.z, eye.x + dir.x, eye.y + dir.y, eye.z + dir.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"mouse press:"<<event->x()<<", "<<event->y();
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();

    update();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos(event->x(), event->y());
    if ( (event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::RightButton))
    {
        m_camera.mouseMove(pos - m_prevMousePos);
    }
    m_prevMousePos = pos;

    update();
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (event->key() == Qt::Key_Space) {
        if (_dir) {
            if (_t < 1.0) {
                _t += _step;
            } else {
                _t -= _step;
                _dir = false;
            }
        } else {
            if (_t > 0.0) {
                _t -= _step;
            } else {
                _t += _step;
                _dir = true;
            }
        }
        _morph->morphTo(_t);
        update();
    }
}

void View::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 2;
    int numSteps = numDegrees;

    m_camera.mouseWheel(numSteps);

    update();
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    // float seconds = time.restart() * 0.001f;

    _t += _step;
    float sint = sin(_t)/2.f + 0.5;
//    _morph->morphTo(sint);

    float cost = cos(_t)/2.f + 0.5;
    float sint2 = sin(_t/2.f)/2.f + 0.5;
    float cost2 = cos(_t/2.f)/2.f + 0.5;
//    Vector3 morphvec = Vector3(sint*cost2,sint*sint2,cost);
//    morphvec.normalize();
//    _3morph->morphTo(Vector3(sint*0.5,cost2*0.5,(1-sint)*0.5+(1-cost2)*0.5));

//    _tick += 1;
//    int limit = std::min((int)_tick,(int)pow(_p,2));
//    for (int i = 0; i < limit; i++) {
//        float t = 1.f;
//        if (_alpha[i] < 1.f) {
//            float t = std::max(_alpha[i],0.01f);
//            t = t*t*(3-2*t);
//        }
//        _alpha[i] = std::min(t*0.01f+_alpha[i],1.0f);
//    }
//    _morph->matrixMorph(_alpha);
    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
