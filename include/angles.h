#pragma once

#ifndef ANGLES_H
#define ANGLES_H

#include "mainHeader.h"
#include "matrix.h"

struct EulerAngles {
    float pitch; // x-rotation
    float yaw;   // y-rotation
    float roll;  // z-rotation
};

Matrix3d EulerToMatrix(const EulerAngles& angle);

#endif /* ANGLE_H */
