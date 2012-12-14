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
    shapes/square.cpp \
    shapes/sphere.cpp \
    shapes/shape.cpp \
    math/CS123Vector.inl \
    math/CS123Matrix.inl \
    math/CS123Matrix.cpp \
    shapes/morpher.cpp \
    camera.cpp \
    shapes/circle.cpp \
    shapes/cylinder.cpp \
    shapes/cone.cpp \
    shapes/torus.cpp \
    shapes/threemorpher.cpp

HEADERS += mainwindow.h \
    view.h \
    shapes/square.h \
    shapes/sphere.h \
    shapes/shape.h \
    math/CS123Algebra.h \
    shapes/morpher.h \
    CS123Common.h \
    camera.h \
    shapes/circle.h \
    shapes/cone.h \
    shapes/torus.h \
    shapes/threemorpher.h

FORMS += mainwindow.ui
INCLUDEPATH += camera math shapes
DEPENDPATH += camera math shapes
