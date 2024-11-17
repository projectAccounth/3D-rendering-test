#include "../include/matrix.h"

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