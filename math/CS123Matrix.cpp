/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

#include "CS123Algebra.h"
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {
    return Matrix4x4(v.x, 0, 0, 0,
                     0, v.y, 0, 0,
                     0, 0, v.z, 0,
                     0, 0, 0, 1);

}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {
    return Matrix4x4(1, 0, 0, v.x,
                     0, 1, 0, v.y,
                     0, 0, 1, v.z,
                     0, 0, 0, 1);

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {
    REAL sinr = sin(radians), cosr = cos(radians);
    return Matrix4x4(1, 0, 0, 0,
                     0, cosr, -sinr, 0,
                     0, sinr, cosr, 0,
                     0, 0, 0, 1);

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {
    REAL sinr = sin(radians), cosr = cos(radians);
    return Matrix4x4(cosr, 0, sinr, 0,
                     0, 1, 0, 0,
                     -sinr, 0, cosr, 0,
                     0, 0, 0, 1);
}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {
    REAL sinr = sin(radians), cosr = cos(radians);
    return Matrix4x4(cosr, -sinr, 0, 0,
                     sinr, cosr, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1);
}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {
    // angles to rotate v to x-axis
    REAL th = atan2(v.z,v.x),
            ph = -atan2(v.y,sqrt(pow(v.z,2)+pow(v.x,2)));

    // all transformations required
    // Note: Inverse of Rot mat is just transpose
    Matrix4x4 T = getInvTransMat(p),
            Tinv = getTransMat(p),
            M1 = getRotYMat(th),
            M2 = getRotZMat(ph),
            M3 = getRotXMat(a);

    return Tinv * M1.getTranspose() * M2.getTranspose()
            * M3 * M2 * M1 * T;
}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {
    return Matrix4x4(1.0/v.x, 0, 0, 0,
                     0, 1.0/v.y, 0, 0,
                     0, 0, 1.0/v.z, 0,
                     0, 0, 0, 1);
}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {
    return Matrix4x4(1, 0, 0, -v.x,
                     0, 1, 0, -v.y,
                     0, 0, 1, -v.z,
                     0, 0, 0, 1);
}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {
    REAL sinr = sin(radians), cosr = cos(radians);
    return Matrix4x4(1, 0, 0, 0,
                     0, cosr, sinr, 0,
                     0, -sinr, cosr, 0,
                     0, 0, 0, 1);
}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {
    REAL sinr = sin(radians), cosr = cos(radians);
    return Matrix4x4(cosr, 0, -sinr, 0,
                     0, 1, 0, 0,
                     sinr, 0, cosr, 0,
                     0, 0, 0, 1);
}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {
    REAL sinr = sin(radians), cosr = cos(radians);
    return Matrix4x4(cosr, sinr, 0, 0,
                     -sinr, cosr, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1);
}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {
    // angles to rotate v to x-axis
    REAL th = atan2(v.z,v.x),
            ph = -atan2(v.y,sqrt(pow(v.z,2)+pow(v.x,2)));

    // all transformations required
    // Note: Inverse of Rot mat is just transpose
    Matrix4x4 T = getInvTransMat(p),
            Tinv = getTransMat(p),
            M1 = getRotYMat(th),
            M2 = getRotZMat(ph),
            M3 = getInvRotXMat(a); // M3^-1 is only diff btw this and regular func

    return Tinv * M1.getTranspose() * M2.getTranspose()
            * M3 * M2 * M1 * T;
}


//@}---------------------------------------------------------------------

