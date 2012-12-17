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
    shapes/threemorpher.cpp \
    jansson/value.c \
    jansson/utf.c \
    jansson/strconv.c \
    jansson/strbuffer.c \
    jansson/pack_unpack.c \
    jansson/memory.c \
    jansson/load.c \
    jansson/hashtable.c \
    jansson/error.c \
    jansson/dump.c \
    shapes/curveloader.cpp \
    shapes/badcube.cpp \
    shapes/cube.cpp \
    shapes/line.cpp \
    animation/animation.cpp \
    animation/cameraanimation.cpp \
    animation/keyframe.cpp

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
    shapes/threemorpher.h \
    jansson/utf.h \
    jansson/strbuffer.h \
    jansson/jansson.h \
    jansson/jansson_private.h \
    jansson/jansson_config.h \
    jansson/hashtable.h \
    shapes/curveloader.h \
    shapes/badcube.h \
    shapes/cube.h \
    shapes/line.h \
    animation/animation.h \
    animation/cameraanimation.h \
    animation/keyframe.h

FORMS += mainwindow.ui
INCLUDEPATH += camera math shapes jansson animation
DEPENDPATH += camera math shapes jansson animation

OTHER_FILES += \
    shaders/pulse.vert \
    shaders/pulse.frag \
    shaders/metal.vert \
    shaders/metal.frag \
    shaders/glass.vert \
    shaders/glass.frag \
    shaders/postop.frag \
    shaders/postop.vert
