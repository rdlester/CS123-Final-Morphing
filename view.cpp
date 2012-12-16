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

    // animation/time parameters
    _t = 0.0;
    _step = 0.05;
    _dir = true;
    _tick = 0;

    // cubemap params
    _skybox = 0;
    _cubeMap = 0;

    // shape params
    _p = 25;
    int pp = (int)pow(_p,2);
    int i;

    // sample matrices for morphing
    _alpha = new float[pp];
    for (i = 0; i < pp; i++) {
        _alpha[i] = (double)rand() / RAND_MAX;
    }

    _alpha3 = new Vector3[pp];



    // morpher arrays
    _numMorph = 4;
    _morph = new Morpher*[_numMorph];
    for (i = 0; i < _numMorph; i++) {
        _morph[i] = NULL;
    }
    _num3Morph = 1;
    _3morph = new ThreeMorpher*[_num3Morph];
    for (i = 0; i < _num3Morph; i++) {
        _3morph[i] = NULL;
    }

    // curve arrays
    _numCurves = 1;
    _curves = new CurveLoader*[_numCurves];
    for (i = 0; i < _numCurves; i++) {
        _curves[i] = NULL;
    }


    // init shapes
    _square = NULL;
    _badcube = NULL;
    _sphere = NULL;
    _circle = NULL;
    _cylinder = NULL;
    _cone = NULL;
    _line = NULL;
}

View::~View()
{
    // clear morpher arrays
    int i;
    for (i = 0; i < _numMorph; i++) {
        delete _morph[i];
    }
    delete[] _morph;
    for (i = 0; i < _num3Morph; i++) {
        delete _3morph[i];
    }
    delete[] _3morph;

    // clear curve array
    for (i = 0; i < _numCurves; i++) {
        delete _curves[i];
    }
    delete[] _curves;

    // delete basic shapes
    delete _square;
    delete _badcube;
    delete _sphere;
    delete _circle;
    delete _cylinder;
    delete _cone;
    delete _line;

    // other stuff
    delete[] _alpha;
    delete[] _alpha3;
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Shoot for ~30fps
    time.start();
    timer.start(33);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Lighting (disable colormaterial when wireframes is enabled
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    // Need to draw back faces for inter-morphing states!
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    setLights();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_CUBE_MAP);

    // for drawing lines
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(GL_CCW);

    glShadeModel(GL_SMOOTH);

    // look
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Additional setup
    loadSkybox();
    loadCubeMap();
    loadShapes();

    // extra parameters that for now should be commented

    //    glEnable(GL_COLOR_MATERIAL);

    // want to show backfaces, leave following uncommented
//    glEnable(GL_CULL_FACE);

    // wireframes, no faces
//    glColor3f(0, 0, 0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void View::setLights()
{
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Vector4(0.25, 0.5, 0.5, 0).data);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Vector4(0.75, 0.75, 0.75, 0).data);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Vector4(1, 1, 1, 0).data);
    glLightfv(GL_LIGHT0, GL_POSITION, Vector4(1, 2, 3, 0).getNormalized().data);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4(1, 1, 1, 0).data);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
}

void View::loadShapes()
{
    int pp = (int)pow(_p,2);
    int i;

    // load shapes
    _line = new Line(_p);
    _square = new Square(_p, "/course/cs123/data/image/topleft.png");
    _badcube = new BadCube(_p);
    _sphere = new Sphere(_p);
    _circle = new Circle(_p);
    _cylinder = new Cylinder(_p);
    _cone = new Cone(_p);

    // load curves
    QString path = View::curvePath() + "newCurve.js";
    _curves[0] = new CurveLoader(_p, path);

    // load morpher
    _morph[0] = new Morpher(_curves[0]->getVertices(), _curves[0]->getNormals(),
                         _square->getVertices(), _square->getNormals(),
                         _p, _square->getTexId());
//    _morph->morphTo(0.0f);
//    _morph->matrixMorph(_alpha);
    _morph[0]->randomMorph();
    _morph[1] = new Morpher(_sphere->getVertices(), _sphere->getNormals(),
                            _line->getVertices(), _line->getNormals(),
                            _p);
    _morph[2] = new Morpher(_circle->getVertices(), _circle->getNormals(),
                             _cone->getVertices(), _cone->getNormals(),
                             _p, _square->getTexId());
    _morph[3] = new Morpher(_curves[0]->getVertices(), _curves[0]->getNormals(),
                             _sphere->getVertices(), _sphere->getNormals(),
                             _p, _square->getTexId());



    // load 3morpher
    _3morph[0] = new ThreeMorpher(_sphere->getVertices(), _sphere->getNormals(),
                               _curves[0]->getVertices(), _curves[0]->getNormals(),
                               _cone->getVertices(), _cone->getNormals(),
                               _p);
    Vector3* coneV = _cone->getVertices();
    for (i = 0; i < pp; i++) {
        Vector3 currV = coneV[i];
        _alpha3[i] = Vector3(currV.x+0.5,currV.y+0.5,currV.z+0.5);
    }
    _3morph[0]->matrixMorph(_alpha3);
}

void View::loadSkybox()
{
    // basically copied from lab 9
    GLuint id = glGenLists(1);
    glNewList(id, GL_COMPILE);

    // Be glad we wrote this for you...ugh.
    glBegin(GL_QUADS);
    float extent = 100.f;
    glTexCoord3f( 1.0f, -1.0f, -1.0f); glVertex3f( extent, -extent, -extent);
    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-extent, -extent, -extent);
    glTexCoord3f(-1.0f,  1.0f, -1.0f); glVertex3f(-extent,  extent, -extent);
    glTexCoord3f( 1.0f,  1.0f, -1.0f); glVertex3f( extent,  extent, -extent);
    glTexCoord3f( 1.0f, -1.0f,  1.0f); glVertex3f( extent, -extent,  extent);
    glTexCoord3f( 1.0f, -1.0f, -1.0f); glVertex3f( extent, -extent, -extent);
    glTexCoord3f( 1.0f,  1.0f, -1.0f); glVertex3f( extent,  extent, -extent);
    glTexCoord3f( 1.0f,  1.0f,  1.0f); glVertex3f( extent,  extent,  extent);
    glTexCoord3f(-1.0f, -1.0f,  1.0f); glVertex3f(-extent, -extent,  extent);
    glTexCoord3f( 1.0f, -1.0f,  1.0f); glVertex3f( extent, -extent,  extent);
    glTexCoord3f( 1.0f,  1.0f,  1.0f); glVertex3f( extent,  extent,  extent);
    glTexCoord3f(-1.0f,  1.0f,  1.0f); glVertex3f(-extent,  extent,  extent);
    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-extent, -extent, -extent);
    glTexCoord3f(-1.0f, -1.0f,  1.0f); glVertex3f(-extent, -extent,  extent);
    glTexCoord3f(-1.0f,  1.0f,  1.0f); glVertex3f(-extent,  extent,  extent);
    glTexCoord3f(-1.0f,  1.0f, -1.0f); glVertex3f(-extent,  extent, -extent);
    glTexCoord3f(-1.0f,  1.0f, -1.0f); glVertex3f(-extent,  extent, -extent);
    glTexCoord3f(-1.0f,  1.0f,  1.0f); glVertex3f(-extent,  extent,  extent);
    glTexCoord3f( 1.0f,  1.0f,  1.0f); glVertex3f( extent,  extent,  extent);
    glTexCoord3f( 1.0f,  1.0f, -1.0f); glVertex3f( extent,  extent, -extent);
    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-extent, -extent, -extent);
    glTexCoord3f(-1.0f, -1.0f,  1.0f); glVertex3f(-extent, -extent,  extent);
    glTexCoord3f( 1.0f, -1.0f,  1.0f); glVertex3f( extent, -extent,  extent);
    glTexCoord3f( 1.0f, -1.0f, -1.0f); glVertex3f( extent, -extent, -extent);
    glEnd();
    glEndList();

    _skybox = id;
}

void View::loadCubeMap()
{
    GLuint id;
    glGenTextures(1,&id);

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    // load texture
    // We're using the same texture on all sides
    QImage image, texture;
    image.load(View::texturePath()+"holodeckMap.png");
    image = image.mirrored(false,true);
    texture = QGLWidget::convertToGLFormat(image);
    texture = texture.scaledToWidth(1024, Qt::SmoothTransformation);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 3, 3,
                     texture.width(), texture.height(), 0, GL_RGBA,GL_UNSIGNED_BYTE,
                     texture.bits());
        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 3,
                          texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                          texture.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    _cubeMap = id;
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

    renderCubeMap();

    drawMorpherGrid();
//    debugShapes();
}

void View::renderCubeMap() {
    glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
    glCallList(_skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
}

void View::drawMorpherGrid()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
//    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);
//    glDisable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float move = 0.5f;
    glPushMatrix();
    glTranslatef(move,move,0.f);
    _morph[0]->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-move,move,0.f);
    _morph[1]->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-move,-move,0.f);
    _morph[2]->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(move,-move,0.f);
    _morph[3]->draw();
    glPopMatrix();
}

void View::debugShapes()
{
    // wireframe
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);
//    _line->draw();
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
//    _morph->draw();
//    _morph->drawNormals();
//    _3morph->draw();
//    _3morph->drawNormals();

    // filled draw
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    _line->draw();
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
    _morph[0]->draw();
//    _morph[0]->drawNormals();
//    _3morph[0]->draw();
//    _3morph[0]->drawNormals();
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
        _morph[0]->morphTo(_t);
        update();
    }
}

void View::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta();
    int numSteps = numDegrees;

    m_camera.mouseWheel(numSteps);

    update();
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    _tick += 1;
    debugCameraAnimation();
    debugMorphAnimation();
    _morph[0]->randomMorph();

    update();
}


void View::debugMorphAnimation()
{
    float sint = sin((double)_tick/15.0)/2.f + 0.5;
    _morph[1]->morphTo(sint);
    if (_tick % 5 == 0) {
        _morph[2]->morphTo((float)rand() / RAND_MAX);
    }

//    float cost = cos(_t)/2.f + 0.5;
//    float sint2 = sin(_t/2.f)/2.f + 0.5;
//    float cost2 = cos(_t/2.f)/2.f + 0.5;
//    Vector3 morphvec = Vector3(sint*cost2,sint*sint2,cost);
//    morphvec.normalize();
//    _3morph[0]->morphTo(Vector3(sint*0.5,cost2*0.5,(1-sint)*0.5+(1-cost2)*0.5));

    int limit = std::min(_tick,(int)pow(_p,2));
    for (int i = 0; i < limit; i++) {
        float t = 1.f;
        if (_alpha[i] < 1.f) {
            float t = std::max(_alpha[i],0.01f);
            t = t*t*(3-2*t);
        }
        _alpha[i] = std::min(t*0.01f+_alpha[i],1.0f);
    }
    _morph[3]->matrixMorph(_alpha);
}

void View::debugCameraAnimation()
{
    float period = 300.0; // length of time to complete one cycle
    float step = _tick/period;
    m_camera.theta = step * 2.0 * M_PI;

    step = std::min(step,1.f);
    //m_camera.phi = (1-step) * 0.2f + step * -M_PI / 2.f;
}
