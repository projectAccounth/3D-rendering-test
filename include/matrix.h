#pragma once

#ifndef MATRIX_H
#define MATRIX_H

#include "mainHeader.h"
#include "vector.h"

class Matrix3d {
public:
	float mat[3][3];

	Matrix3d operator+(const Matrix3d& other) const;

	Matrix3d operator-(const Matrix3d& other) const;

	Matrix3d operator*(const Matrix3d& other) const;

	Vector3 operator*(const Vector3& other) const;
};

#endif /* MATRIX_H */
