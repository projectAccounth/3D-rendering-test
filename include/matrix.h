#pragma once

#ifndef MATRIX_H
#define MATRIX_H

#include "mainHeader.h"
#include "vector.h"
#include "point.h"

class Matrix3d {
public:
	double mat[3][3];

	Matrix3d operator+(const Matrix3d& other) const;

	Matrix3d operator-(const Matrix3d& other) const;

	Matrix3d operator*(const Matrix3d& other) const;

	Vector3 operator*(const Vector3& other) const;

	Point3 operator*(const Point3& other) const;
};

#endif /* MATRIX_H */
