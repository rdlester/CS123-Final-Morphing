#ifndef KEYFRAME_H
#define KEYFRAME_H

class Keyframe
{
public:
    Keyframe(int start, int end);
    ~Keyframe();

    inline int getStart() { return _start; }
    inline int getEnd() { return _end; }

protected:
    int _start, _end;
};

#endif // KEYFRAME_H
