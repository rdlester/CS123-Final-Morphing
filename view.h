#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <QGLShaderProgram>
#include <QGLFramebufferObject>
#include <QHash>
#include <line.h>
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

    static const QString curvePath()
    {
        QString path = "/Users/Ryan/Documents/Brown/Masters1/GFX/final/CS123-Final-Morphing/curves/";
        return path;
    }
    static const QString texturePath()
    {
        QString path = "/Users/Ryan/Documents/Brown/Masters1/GFX/final/CS123-Final-Morphing/textures/";
        return path;
    }
    static const QString shaderPath()
    {
        QString path = "/Users/Ryan/Documents/Brown/Masters1/GFX/final/CS123-Final-Morphing/shaders/";
        return path;
    }

private slots:
    void tick();

private:
    QTime time;
    QTimer timer;

    //! Initialization routines
    void initializeGL();
    void setLights();
    void loadTextures();
    GLuint loadTexture(QString texturePath);
    void loadShapes();
    void loadSkybox();
    void loadCubeMap();
    void createShaderPrograms();
    void createFramebufferObjects();

    //! rendering routines
    void paintGL();
    //! Camera settings
    void applyPerspectiveCamera(float width, float height);
    //! CubeMap creation
    void renderCubeMap();
    //! Debug draw, grid of morphers
    void drawMorpherGrid();
    //! Helper to draw wireframes or fill of different shapes
    void debugShapes();


    void enableFill();
    void enableWireframe();

    // Animation
    //! Basic animation of morph shapes, run on tick
    void debugMorphAnimation();
    //! Basic animation of orbit camera
    void debugCameraAnimation();

    void resizeGL(int w, int h);

    //! Interaction handlers
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    //////////////
    // Parameters!

    // camera controls
    OrbitCamera m_camera;
    GLUquadric* m_quadric;
    Vector2 m_prevMousePos;

    // cubemap
    int _numTex;
    GLuint* _texIds;
    GLuint _skybox;
    GLuint _cubeMap;

    // Post-processing, shaders
    QHash<QString, QGLShaderProgram *> _shaderPrograms;
    QHash<QString, QGLFramebufferObject *> _framebufferObjects;

    // shape param
    int _p;

    // animation params
    double _t;
    double _step;
    bool _dir;
    int _tick;

    // arrays of morphers
    int _numMorph;
    int _num3Morph;
    Morpher** _morph;
    ThreeMorpher** _3morph;
    float* _alpha;
    Vector3* _alpha3;

    // array of loaded curves
    int _numCurves;
    CurveLoader** _curves;

    // simple shapes
    Line* _line;
    Square* _square;
    BadCube* _badcube;
    Sphere* _sphere;
    Circle* _circle;
    Cylinder* _cylinder;
    Cone* _cone;
};

#endif // VIEW_H

