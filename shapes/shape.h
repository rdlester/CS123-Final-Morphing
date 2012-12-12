#ifndef SHAPE_H
#define SHAPE_H

#include "gl.h"

/**
 * Interface for shapes
 * Stores array of vertices and normals
 * Subclasses must implement drawShape() and setParams(int p1, int p2)
 */
class Shape
{
public:

    Shape();
    virtual ~Shape();

    /**
      * Draws the shape on screen centered at the origin
      */
    virtual void draw() = 0;

    /**
      * Sets the tesselation parameters of the object
      */
    void setParams(int p1, int p2);

    /**
      * Updates the internal representation of the shape
      * to enable fast drawing
      */
    virtual void build() = 0;

    /**
      * Getters for shape definition
      * Used in shape blending code
      */
    virtual Vector3* getVertices() = 0;
    virtual Vector3* getNormals() = 0;
    virtual int getNumVertices() = 0;

protected:
    // Store tesselation parameters
    int _p1, _p2;
};

#endif // SHAPE_H
