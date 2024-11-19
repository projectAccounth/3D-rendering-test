#pragma once

#ifndef ANGLES_H
#define ANGLES_H

#include "mainHeader.h"
#include "matrix.h"
#include "point.h"

struct EulerAngles {
    double pitch; // x-rotation
    double yaw;   // y-rotation
    double roll;  // z-rotation
};

Matrix3d EulerToMatrix(const EulerAngles& angle);

Matrix3d EulerToMatrixLH(const EulerAngles& angle);


void movePointForward(Point3& pointPosition, EulerAngles &angles, double distance);

void movePointRight(Point3& pointPosition, EulerAngles &angles, double distance);

void movePointBack(Point3& pointPosition, EulerAngles &angles, double distance);

void movePointLeft(Point3& pointPosition, EulerAngles &angles, double distance);


#endif /* ANGLE_H */
