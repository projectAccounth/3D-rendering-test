#include "../include/matrix.h"
#include "../include/point.h"

Matrix3d Matrix3d::operator*(const Matrix3d& other) const {
    Matrix3d result;

    for (int i = 0; i < 3; ++i) { 
        for (int j = 0; j < 3; ++j) {
            result.mat[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                result.mat[i][j] += mat[i][k] * other.mat[k][j];
            }
        }
    }

    return result;
}

Matrix3d Matrix3d::operator+(const Matrix3d& other) const {
    Matrix3d result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mat[i][j] = mat[i][j] + other.mat[i][j];
        }
    }
    return result;
}

Matrix3d Matrix3d::operator-(const Matrix3d& other) const {
    Matrix3d result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mat[i][j] = mat[i][j] - other.mat[i][j];
        }
    }
    return result;
}

Vector3 Matrix3d::operator*(const Vector3& vector) const {
    Vector3 result;

    result.x = mat[0][0] * vector.x + mat[0][1] * vector.y + mat[0][2] * vector.z;
    result.y = mat[1][0] * vector.x + mat[1][1] * vector.y + mat[1][2] * vector.z;
    result.z = mat[2][0] * vector.x + mat[2][1] * vector.y + mat[2][2] * vector.z;

    return result;
}

Point3 Matrix3d::operator*(const Point3& other) const {
    Point3 result;

    result.x = mat[0][0] * other.x + mat[0][1] * other.y + mat[0][2] * other.z;
    result.y = mat[1][0] * other.x + mat[1][1] * other.y + mat[1][2] * other.z;
    result.z = mat[2][0] * other.x + mat[2][1] * other.y + mat[2][2] * other.z;

    return result;
}