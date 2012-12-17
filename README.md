CS123-Final
====================

yunmiao (yun_miao(at)brown.edu)
rdlester (RL(at)brown.edu)

Our CS123 final project consists of two parts: a modeler UI for creating bezier, bspline, and flat surfaces; and a short animation featuring these shapes and others morphing into new shapes.

The modeler UI features bezier+bspline surfaces and bump mapping (using a shader). The animation uses the surfaces, and also adds morphing shapes and animation.

The modeler UI was partly inspired by a project for Berkeley's GFX course (http://inst.eecs.berkeley.edu/~cs184/sp12/assignments/hw4.pdf).

To run:
Open in QtCreator and build. You should be able to run either program without much fuss. If you run into trouble or things aren't looking like they're supposed to, you may have to edit file paths within the programs.
In the morpher animation, these paths are found towards the top of 'view.h'. You'll want to edit the paths in the '#ifndef __APPLE__' block to the paths leading into the project folder on your computer.
In the modeler UI, the paths are found in 'view3d.cpp', at the bottom of 'initializeGL()' and in 'createShaderPrograms()'.