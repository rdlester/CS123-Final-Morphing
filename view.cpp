#include "view.h"
#include <QApplication>
#include <QKeyEvent>
#include <QFile>

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

    m_camera.center = Vector3(0.f, -7.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 4.f;
    m_camera.theta = M_PI * 0.5f, m_camera.phi = -M_PI/2.0+0.2;
    m_camera.fovy = 60.f;

    m_quadric = gluNewQuadric();

    // animation/time parameters
    _t = 0.0;
    _step = 0.05;
    _dir = true;
    _tick = 0;

    _frameRate = 30;
    _numEvents = 11;
    _score = new int[_numEvents];
    _score[0] = 2 * _frameRate; // fly-in, 4sec
    _score[1] = _score[0] + 3 * _frameRate; // face wall, 3sec
    _score[2] = _score[1] + 6 * _frameRate; // morph to sphere, 3sec
    _score[3] = _score[2] + (int)(0.25 * _frameRate); // hold, .5sec
    _score[4] = _score[3] + 3 * _frameRate; // morph to cone, 3.5sec
    _score[5] = _score[4] + 3 * _frameRate; // morph to cylinder, 3sec
    _score[6] = _score[5] + 19 * _frameRate; // morph to curves, 15sec
    _score[7] = _score[6] + 1 * _frameRate; // hold, 2sec
    _score[8] = _score[7] + 25 * _frameRate; // spaz and morph inners, 20sec
    _score[9] = _score[8] + 12 * _frameRate; // morph spazzers, 10sec
    _score[10] = _score[9] + 5 * _frameRate; // disappear, 5sec
    _currEvent = 0;

    // 2d texture params
    _numTex = 5;
    _texIds = new GLuint[_numTex];

    // cubemap params
    _skybox = 0;
    _cubeMap = 0;

    // shape params
    _p = 31;
    int pp = (int)pow(_p,2);
    int i;

    // sample matrices for morphing
    _alpha = new float[pp];
    for (i = 0; i < pp; i++) {
        _alpha[i] = 0;
    }

    _alpha3 = new Vector3[pp];

    // morpher arrays
    _numMorph = 18;
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
    _numCurves = 6;
    _curves = new CurveLoader*[_numCurves];
    for (i = 0; i < _numCurves; i++) {
        _curves[i] = NULL;
    }

    // grid
    _gridDim = 6;
    _spacing = 1.f;
    _grid = new GridEntry[_gridDim*_gridDim];

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

    // clear grid
    delete[] _grid;
    delete[] _score;

    glDeleteTextures(_numTex, _texIds);
    glDeleteTextures(1, &_cubeMap);
    glDeleteLists(_skybox, 1);
    gluDeleteQuadric(m_quadric);

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

    // Lighting
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    setLights();

    // have to enable texture type one at a time!
//    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_TEXTURE_CUBE_MAP);

    glEnable(GL_BLEND);

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
    createShaderPrograms();
    createFramebufferObjects();
    loadSkybox();
    loadCubeMap();
    loadTextures();
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
    glLightfv(GL_LIGHT0, GL_AMBIENT, Vector4(0.4, 0.6, 0.6, 0).data);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Vector4(1.0, 1.0, 1.0, 0).data);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Vector4(1, 1, 1, 0).data);
    glLightfv(GL_LIGHT0, GL_POSITION, Vector4(1, 2, 3, 0).getNormalized().data);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4(1, 1, 1, 0).data);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
}

void View::loadTextures()
{
    glEnable(GL_TEXTURE_2D);
    _texIds[0] = loadTexture("/course/cs123/data/image/circuit.png");
    _texIds[1] = loadTexture("/course/cs123/data/image/marble.png");
    _texIds[2] = loadTexture("/course/cs123/data/image/earth.png");
    _texIds[3] = loadTexture("/course/cs123/data/image/tenebrous3.jpg");
    _texIds[4] = loadTexture("/course/cs123/data/image/fur1.jpg");
    glDisable(GL_TEXTURE_2D);
}

GLuint View::loadTexture(QString texturePath)
{
    // Make sure the image file exists
    QFile file(texturePath);
    if (!file.exists()) {
        return -1;
    }

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Create and load new texture
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(),
                      GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

void View::loadShapes()
{
    int pp = (int)pow(_p,2);
    int i;

    // load shapes
    _line = new Line(_p);
    _square = new Square(_p);
    _badcube = new BadCube(_p);
    _sphere = new Sphere(_p);
    _circle = new Circle(_p);
    _cylinder = new Cylinder(_p);
    _cone = new Cone(_p);

    // load curves
    _curves[0] = new CurveLoader(_p, View::curvePath() + "hat.js");
    _curves[1] = new CurveLoader(_p, View::curvePath() + "mushroom.js");
    _curves[2] = new CurveLoader(_p, View::curvePath() + "tree.js");
    _curves[3] = new CurveLoader(_p, View::curvePath() + "vase.js");
    _curves[4] = new CurveLoader(_p, View::curvePath() + "newCurve.js");
    _curves[5] = new CurveLoader(_p, View::curvePath() + "cup.js");


    // load morpher
    _morph[0] = new Morpher(_square->getVertices(), _square->getNormals(),
                         _sphere->getVertices(), _sphere->getNormals(),
                         _p);
    _morph[1] = new Morpher(_sphere->getVertices(), _sphere->getNormals(),
                            _cone->getVertices(), _cone->getNormals(),
                            _p);
    _morph[2] = new Morpher(_cone->getVertices(), _cone->getNormals(),
                             _cylinder->getVertices(), _cylinder->getNormals(),
                             _p);
    _morph[3] = new Morpher(_cylinder->getVertices(), _cylinder->getNormals(),
                             _curves[0]->getVertices(), _curves[0]->getNormals(),
                             _p);
    _morph[4] = new Morpher(_cylinder->getVertices(), _cylinder->getNormals(),
                             _curves[1]->getVertices(), _curves[1]->getNormals(),
                             _p);
    _morph[5] = new Morpher(_cylinder->getVertices(), _cylinder->getNormals(),
                             _curves[2]->getVertices(), _curves[2]->getNormals(),
                             _p);
    _morph[6] = new Morpher(_cylinder->getVertices(), _cylinder->getNormals(),
                             _curves[3]->getVertices(), _curves[3]->getNormals(),
                             _p);
    _morph[7] = new Morpher(_cylinder->getVertices(), _cylinder->getNormals(),
                             _curves[4]->getVertices(), _curves[4]->getNormals(),
                             _p);
    _morph[8] = new Morpher(_cylinder->getVertices(), _cylinder->getNormals(),
                             _curves[5]->getVertices(), _curves[5]->getNormals(),
                             _p);
    _morph[9] = new Morpher(_curves[1]->getVertices(), _curves[1]->getNormals(),
                            _sphere->getVertices(), _sphere->getNormals(),
                             _p);
    _morph[10] = new Morpher(_curves[2]->getVertices(), _curves[2]->getNormals(),
                             _curves[0]->getVertices(), _curves[0]->getNormals(),
                             _p);
    _morph[11] = new Morpher(_curves[3]->getVertices(), _curves[3]->getNormals(),
                             _curves[5]->getVertices(), _curves[5]->getNormals(),
                             _p);
    _morph[12] = new Morpher(_curves[4]->getVertices(), _curves[4]->getNormals(),
                             _cone->getVertices(), _cone->getNormals(),
                             _p);
    _morph[13] = new Morpher(_curves[4]->getVertices(), _curves[4]->getNormals(),
                             _line->getVertices(), _line->getNormals(),
                             _p);
    _morph[14] = new Morpher(_sphere->getVertices(), _sphere->getNormals(),
                             _line->getVertices(), _line->getNormals(),
                             _p);
    _morph[15] = new Morpher(_curves[0]->getVertices(), _curves[0]->getNormals(),
                             _line->getVertices(), _line->getNormals(),
                             _p);
    _morph[16] = new Morpher(_curves[5]->getVertices(), _curves[5]->getNormals(),
                             _line->getVertices(), _line->getNormals(),
                             _p);
    _morph[17] = new Morpher(_cone->getVertices(), _cone->getNormals(),
                             _line->getVertices(), _line->getNormals(),
                             _p);


    // load 3morpher
    _3morph[0] = new ThreeMorpher(_sphere->getVertices(), _sphere->getNormals(),
                               _curves[0]->getVertices(), _curves[0]->getNormals(),
                               _cone->getVertices(), _cone->getNormals(),
                               _p);

    // initialize grid
    for (i = 0; i < _gridDim*_gridDim; i++) {
        _grid[i] = GridEntry(_square, 0, false);
    }
}

void View::loadSkybox()
{
    // basically copied from lab 9
    GLuint id = glGenLists(1);
    glNewList(id, GL_COMPILE);

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
    glEnable(GL_TEXTURE_CUBE_MAP);
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
    glDisable(GL_TEXTURE_CUBE_MAP);

    _cubeMap = id;
}

void View::createShaderPrograms()
{
//    const QGLContext *ctx = context();

////    QGLShaderProgram* glass = new QGLShaderProgram(ctx);
////    glass->addShaderFromSourceFile(QGLShader::Vertex, View::shaderPath()+"glass.vert");
////    glass->addShaderFromSourceFile(QGLShader::Fragment, View::shaderPath()+"glass.frag");
////    glass->link();
////    _shaderPrograms["glass"] = glass;

////    QGLShaderProgram* metal = new QGLShaderProgram(ctx);
////    metal->addShaderFromSourceFile(QGLShader::Vertex, View::shaderPath()+"metal.vert");
////    metal->addShaderFromSourceFile(QGLShader::Fragment, View::shaderPath()+"metal.frag");
////    metal->link();
////    _shaderPrograms["metal"] = metal;

//    QGLShaderProgram* postop = new QGLShaderProgram(ctx);
//    postop->addShaderFromSourceFile(QGLShader::Fragment, View::shaderPath()+"postop.frag");
//    bool linked = postop->link();
//    if (!linked) std::cout << postop->log().toStdString() << "\n";
//    _shaderPrograms["postop"] = postop;
}

// Basically copied from lab 9
void View::createFramebufferObjects()
{
    // Allocate the main framebuffer object for rendering the scene to
    // This needs a depth attachment
//    _framebufferObjects["0"] = new QGLFramebufferObject(width(), height(),
//                                            QGLFramebufferObject::Depth,
//                                            GL_TEXTURE_2D, GL_RGB16F_ARB);
//    _framebufferObjects["0"]->format().setSamples(16);
//    // Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
//    // These do not require depth attachments
//    _framebufferObjects["1"] = new QGLFramebufferObject(width(), height(),
//                                            QGLFramebufferObject::NoAttachment,
//                                            GL_TEXTURE_2D, GL_RGB16F_ARB);

//    _framebufferObjects["2"] = new QGLFramebufferObject(width(), height(),
//                                            QGLFramebufferObject::NoAttachment,
//                                            GL_TEXTURE_2D, GL_RGB16F_ARB);
}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    applyPerspectiveCamera(width(), height());
    renderCubeMap();
    drawMorpherGrid();

//    _framebufferObjects["0"]->bind();
//    applyPerspectiveCamera(this->width(),this->height());
//    renderCubeMap();
//    drawMorpherGrid();
//    _framebufferObjects["0"]->release();

//    _framebufferObjects["0"]->blitFramebuffer(_framebufferObjects["1"],
//                                                  QRect(0, 0, width(), height()), _framebufferObjects["0"],
//                                                  QRect(0, 0, width(), height()), GL_COLOR_BUFFER_BIT, GL_NEAREST);

//    // Post-processing
//    glEnable(GL_TEXTURE_2D);
//    // camera stuff
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0.f, width(), 0.f, height());
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    // draw the framebuffer
////    _shaderPrograms["postop"]->bind();
//    glBindTexture(GL_TEXTURE_2D, _framebufferObjects["1"]->texture());
//    // draw to quad
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0f, 0.0f);
//    glVertex2f(0.0f, 0.0f);
//    glTexCoord2f(1.0f, 0.0f);
//    glVertex2f(width(), 0.0f);
//    glTexCoord2f(1.0f, 1.0f);
//    glVertex2f(width(), height());
//    glTexCoord2f(0.0f, 1.0f);
//    glVertex2f(0.0f, height());
//    glEnd();
////    _shaderPrograms["postop"]->release();
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glDisable(GL_TEXTURE_2D);
}

void View::renderCubeMap() {
    glEnable(GL_TEXTURE_CUBE_MAP);
    enableFill();
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
    glCallList(_skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

void View::drawMorpherGrid()
{
    glEnable(GL_TEXTURE_2D);

    float start = -_spacing*(_gridDim/2.f-1)-_spacing/2.f;


    int i,j;
    for (i = 0; i < _gridDim; i++) {
        float y = start + i*_spacing;
        for (j = 0; j < _gridDim; j++) {
            float x = start + j*_spacing;
            glPushMatrix();
            glTranslatef(x,y,0.f);
            GridEntry* curr = _grid + i*_gridDim+j;
            if (curr->fill) {
                enableFill();
            } else {
                enableWireframe();
            }
            glBindTexture(GL_TEXTURE_2D, curr->texture);
            curr->entry->draw();
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();
        }
    }
//    glBindTexture(GL_TEXTURE_2D, _texIds[4]);
//    enableFill();
//    glPushMatrix();
//    glTranslatef(move,move,0.f);
//    _curves[0]->draw();
////    _morph[0]->draw();
//    glPopMatrix();


//    glBindTexture(GL_TEXTURE_2D, _texIds[2]);
//    enableFill();
//    glPushMatrix();
//    glTranslatef(-move,move,0.f);
//    _morph[1]->draw();
//    glPopMatrix();

//    glBindTexture(GL_TEXTURE_2D, _texIds[1]);
//    enableFill();
//    glPushMatrix();
//    glTranslatef(-move,-move,0.f);
//    _morph[2]->draw();
//    glPopMatrix();

//    enableWireframe();
//    glPushMatrix();
//    glTranslatef(move,-move,0.f);
//    _morph[3]->draw();
//    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void View::debugShapes()
{
    // wireframe
    enableWireframe();
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
    enableFill();
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

void View::enableFill()
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void View::enableWireframe()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);
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

void View::tick()
{
    _tick += 1;

    /////////////////////////////
    // scheduling+animation logic

    // update current event
    reachedNextEvent();

    switch(_currEvent) {
    case 0: // flyin
    {
        float start = -7.f;
        float end = -2.f;
        float t = _tick / (float)_score[0];
        m_camera.center.y = (1-t)*start + t*end;
        break;
    }
    case 1: // face wall
    {
        float t = (_tick-_score[0]) / (float)(_score[1]-_score[0]);

        float starty = -2.f;
        float endy = 0.f;
        m_camera.center.y = starty - (endy-starty)*t*(t-2.f); // cubic easing out

        // tilt camera up
        t *= 2;
        float startphi = -M_PI/2.0+0.2;
        float endphi = 0.1f;
        if (t < 1) {
            m_camera.phi = startphi + ((endphi - startphi)/2.f)*(t*t);
        } else {
            t--;
            m_camera.phi = startphi - ((endphi - startphi)/2.f)*(t*(t-2)-1);
        }
        break;
    }
    case 2: // morph to sphere
    {
        if (_tick == _score[1]) {
            int i;
            for (i = 0; i < _gridDim*_gridDim; i++) {
                // the first morphing!
                _grid[i].entry = _morph[0];
            }
        }
        float t = (_tick-_score[1]) / (float)(_score[2]-_score[1]);
        float sint = 1.0 - (sin(t*M_PI+M_PI/2.0)/2.0 + 0.5);
        _morph[0]->morphTo(sint);

        break;
    }
    case 3: // hold
    {

        break;
    }
    case 4: // to cone
    {
        if (_tick == _score[3]) {
            int i;
            for (i = 0; i < _gridDim*_gridDim; i++) {
                _grid[i].entry = _morph[1];
                _grid[i].fill = true;
            }
        }
        float t = (_tick-_score[3]) / (float)(_score[4]-_score[3]);
        t = t*t*t;
        _morph[1]->morphTo(t);

        break;
    }
    case 5: // to cylinder
    {
        if (_tick == _score[4]) {
            int i;
            for (i = 0; i < _gridDim*_gridDim; i++) {
                _grid[i].entry = _morph[2];
                _grid[i].texture = _texIds[3]; // TODO
            }
        }
        float t = (_tick-_score[4]) / (float)(_score[5]-_score[4]);
        t--;
        t = t*t*t+1;
        std::cout << t << "\n";
        _morph[2]->morphTo(t);

        break;
    }
    case 6: // to curves
    {
        if (_tick == _score[5]) {
            int i,j;
            for (i = 0; i < _gridDim; i++) {
                for (j = 0; j < _gridDim;j++) {
                    _grid[i*_gridDim+j].entry = _morph[3+j]; // cylinder to curve
                    _grid[i*_gridDim+j].fill = true;
                    GLuint tex = 0;
                    switch(j) {
                    case 0:
                        tex = _texIds[4];
                        break;
                    case 1:
                        tex = _texIds[2];
                        break;
                    case 2:
                        tex = 0;
                        break;
                    case 3:
                        tex = _texIds[1];
                        break;
                    case 4:
                        tex = 0;
                        break;
                    case 5:
                        tex = 0;
                        break;
                    }

                    _grid[i*_gridDim+j].texture = tex;
                }
            }
        }
        float t = (_tick-_score[5]) / (float)(_score[6]-_score[5]);
        float sint = 1.0 - (sin(t*M_PI+M_PI/2.0)/2.0 + 0.5);
        for (int i = 0; i < _gridDim; i++) {
            _morph[3+i]->morphTo(sint);
        }

        break;
    }
    case 7: // hold
    {

        break;
    }
    case 8: // spaz out edges!
    {
        if (_tick == _score[7]) {
            int i,j;
            for (i = 0; i < _gridDim; i++) {
                for (j = 0; j < _gridDim; j++) {
                    if (i == 0 || j == 0 || i == _gridDim-1 || j == _gridDim-1) {
                        _grid[i*_gridDim+j].entry = _morph[7];
                    } else {
                        _grid[i*_gridDim+j].entry = _morph[8+j]; // need four more
                    }
                }
            }
            _morph[7]->randomMorph();
        }
        if (_tick % 39 == 0 || _tick % 33 == 0) {
            _morph[7]->randomMorph();
        }

        // update the morph matrix
        int pp = (int)pow(_p,2);
        // calculate fraction active
        int end = (_score[8]-_score[7]);
        float frac = std::min(10*(_tick-_score[7]+1)/(float)(9*end),1.f);
        int active = frac*pp;

        // move active fraction
        int i;
        for (i = 0; i < active; i++) {
            // calculate the step index was activated on
            int stepactive = i*9*end / (10*pp*pp);
            float t = (_tick-_score[7]-stepactive) / (float)(end);
            float sint = 1.0 - (sin(t*M_PI+M_PI/2.0)/2.0 + 0.5);
            _alpha[i] = sint;
        }

        _morph[9]->matrixMorph(_alpha);
        _morph[10]->matrixMorph(_alpha);
        _morph[11]->matrixMorph(_alpha);
        _morph[12]->matrixMorph(_alpha);

        break;
    }
    case 9:
    {
        int i, pp = (int)pow(_p,2);
        if (_tick == _score[8]) {
            for (i = 0; i < pp; i++) {
                _alpha[i] = (float)rand() / RAND_MAX;
            }
        }

        for (i = 0; i < pp; i++) {
            // converging towards actual shape but never reaching
            float step = 1.f-_alpha[i];
            _alpha[i] = std::min(0.01f*step+_alpha[i],1.f);
        }

        _morph[7]->matrixMorph(_alpha);

        break;
    }
    case 10:
        if (_tick == _score[9]) {
            int i,j;
            for (i = 0; i < _gridDim; i++) {
                for (j = 0; j < _gridDim; j++) {
                    if (i == 0 || j == 0 || i == _gridDim-1 || j == _gridDim-1) {
                        _grid[i*_gridDim+j].entry = _morph[13];
                    } else {
                        _grid[i*_gridDim+j].entry = _morph[13+j]; // need four more
                    }
                }
            }
        }
        float t = (_tick-_score[9]) / (float)(_score[10]-_score[9]);
        float sint = 1.0 - (sin(t*M_PI+M_PI/2.0)/2.0 + 0.5);
        _morph[13]->morphTo(sint);
        _morph[14]->morphTo(sint);
        _morph[15]->morphTo(sint);
        _morph[16]->morphTo(sint);
        _morph[17]->morphTo(sint);
        break;
    }

//    debugCameraAnimation();
//    debugMorphAnimation();
//    _morph[0]->randomMorph();

    update();
}

void View::reachedNextEvent()
{
    // if we've reached the end of an event go to next one
    if (_tick >= _score[_currEvent]) {
        _currEvent += 1;
    }
}

void View::debugMorphAnimation()
{
    float sint = sin((double)_tick/15.0)/2.f + 0.5;
    _morph[0]->morphTo(sint);
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
    float period = 60.0; // length of time to complete one cycle
    float step = sin(_tick/period * M_PI);
    m_camera.center.y = m_camera.center.y - 0.01 * step;

//    step = std::min(step,1.f);
    //m_camera.phi = (1-step) * 0.2f + step * -M_PI / 2.f;
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
