#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
public:
    Animation(int length);
    ~Animation();

    /**
     * Running animation is two step process
     * First call setup
     * Then step through
     * Step returns true when end is reached
     * Calling setup in middle of animation resets
     */
    virtual void setup();
    virtual bool step();
protected:
    int _step;
    int _length;
};

#endif // ANIMATION_H
