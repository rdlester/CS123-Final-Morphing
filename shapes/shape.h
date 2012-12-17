#ifndef SHAPE_H
#define SHAPE_H

#ifdef __APPLE__
#include "gl.h"
#endif
#ifndef __APPLE__
#include "GL/gl.h"
#endif
#include <CS123Algebra.h>

/**
 * Interface for shapes
 * Stores array of vertices and normals
 * Subclasses must implement drawShape() and setParams(int p1, int p2)
 */
class Shape
{
public:

    Shape(QString texturePath);
    Shape(int texId);
    virtual ~Shape();

    int loadTexture(QString texturePath);

    /**
      * Draws the shape on screen centered at the origin
      */
    void draw();

    //! Helper function for debugging normals
    void drawNormals();

    /**
      * Sets the tesselation parameters of the object
      */
    void setParams(int p);

    /**
      * Updates the internal representation of the shape
      * to enable fast drawing
      */
    virtual void build();

    /**
      * Getters for shape definition
      * Used in shape blending code
      */
    Vector3* getVertices();
    Vector3* getNormals();
    int getNumVertices();
    int getTexId();

protected:
    // Store tesselation parameters
    int _p;
    Vector3* _vertices;
    Vector3* _normals;
    int _texId;
};

#endif // SHAPE_H
