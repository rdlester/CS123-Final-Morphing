# 
# CS123 Final Project Starter Code
# Adapted from starter code graciously provided by CS195-U: 3D Game Engines
#

QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
# INCLUDEPATH += folder1 folder2
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    shapes/torus.cpp \
    shapes/square.cpp \
    shapes/sphere.cpp \
    shapes/shape.cpp \
    shapes/mobius.cpp \
    shapes/cylinder.cpp \
    shapes/cube.cpp \
    shapes/cone.cpp \
    shapes/circle.cpp \
    camera/OrbitingCamera.cpp \
    math/CS123Vector.inl \
    math/CS123Matrix.inl \
    math/CS123Matrix.cpp

HEADERS += mainwindow.h \
    view.h \
    shapes/torus.h \
    shapes/square.h \
    shapes/sphere.h \
    shapes/shape.h \
    shapes/mobius.h \
    shapes/cylinder.h \
    shapes/cube.h \
    shapes/cone.h \
    shapes/circle.h \
    camera/OrbitingCamera.h \
    camera/Camera.h \
    math/CS123Algebra.h

FORMS += mainwindow.ui
