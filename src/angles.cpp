#include "../include/angles.h"

// 2 totally different implementation!!

Matrix3d EulerToMatrix(const EulerAngles& angles) {
    double cp = std::cos(angles.pitch);
    double sp = std::sin(angles.pitch);
    double cy = std::cos(angles.yaw);
    double sy = std::sin(angles.yaw);
    double cr = std::cos(angles.roll);
    double sr = std::sin(angles.roll);

    Matrix3d matrix;

    matrix.mat[0][0] = cy * cr;
    matrix.mat[0][1] = -cy * sr;
    matrix.mat[0][2] = sy;

    matrix.mat[1][0] = sp * sy * cr + cp * sr;
    matrix.mat[1][1] = -sp * sy * sr + cp * cr;
    matrix.mat[1][2] = -sp * cy;

    matrix.mat[2][0] = -cp * sy * cr + sp * sr;
    matrix.mat[2][1] = cp * sy * sr + sp * cr;
    matrix.mat[2][2] = cp * cy;

    return matrix;
}

Matrix3d EulerToMatrixLH(const EulerAngles& angle) {
    Matrix3d result;

    double sinX = std::sin(angle.pitch), cosX = std::cos(angle.pitch);
    double sinY = std::sin(angle.yaw), cosY = std::cos(angle.yaw);
    double sinZ = std::sin(angle.roll), cosZ = std::cos(angle.roll);

    Matrix3d Rx = { {
        {1, 0, 0},
        {0, cosX, sinX},
        {0, -sinX, cosX}
    } };

    Matrix3d Ry = { {
        {cosY, 0, -sinY},
        {0, 1, 0},
        {sinY, 0, cosY}
    } };

    Matrix3d Rz = { {
        {cosZ, sinZ, 0},
        {-sinZ, cosZ, 0},
        {0, 0, 1}
    } };

    // Combine rotations: XYZ
    result = Rx * Ry * Rz;

    return result;
}