#include "animation.h"

Animation::Animation(int length) : _step(0), _length(length)
{
}

Animation::~Animation()
{

}

void Animation::setup()
{
    // reset step count
    _step = 0;
}

bool Animation::step()
{
    _step += 1;
    if (_step >= _length) {
        return true;
    }
    return false;
}
