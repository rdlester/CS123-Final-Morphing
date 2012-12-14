#include "camera.h"
#include <qgl.h>
#include "math/CS123Algebra.h"
#include "QDebug"
void OrbitCamera::mouseMove(const Vector2 &delta)
{

    //qDebug()<<"camera: "<<delta.x<<", "<<delta.y;
    // Rotate the eye vector around the origin
    theta += delta.x * 0.01f;
    phi += delta.y * 0.01f;

    // Keep theta in [0, 2pi] and phi in [-pi/2, pi/2]
    theta -= floorf(theta / (2*M_PI)) * (2*M_PI);
    phi = fmax(0.01f - M_PI / 2, fmin(M_PI / 2 - 0.01f, phi));
}

void OrbitCamera::mouseWheel(float delta)
{
    zoom *= powf(0.999f, delta);
}
