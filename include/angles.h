#pragma once

#ifndef ANGLES_H
#define ANGLES_H

#include "mainHeader.h"
#include "matrix.h"
#include "point.h"

struct EulerAngles {
    float pitch; // x-rotation
    float yaw;   // y-rotation
    float roll;  // z-rotation
};

Matrix3d EulerToMatrix(const EulerAngles& angle);

Matrix3d EulerToMatrixLH(const EulerAngles& angle);


void movePointForward(Point3& pointPosition, EulerAngles &angles, float distance);

void movePointRight(Point3& pointPosition, EulerAngles &angles, float distance);

void movePointBack(Point3& pointPosition, EulerAngles &angles, float distance);

void movePointLeft(Point3& pointPosition, EulerAngles &angles, float distance);


#endif /* ANGLE_H */
