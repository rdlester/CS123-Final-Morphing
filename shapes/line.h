#ifndef LINE_H
#define LINE_H

#include <shape.h>

class Line : public Shape
{
public:
    Line(int p);
    // no more constructors: lines can't have textures!
    ~Line();
    void build();
};

#endif // LINE_H
