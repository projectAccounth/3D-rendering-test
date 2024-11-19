#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include "mainHeader.h"
#include "point.h"

class Vector2 {
public:
	double x, y;

	Vector2() : x(0), y(0) {}
	Vector2(double x, double y) : x(x), y(y) {}

	double dot(const Vector2& vec) const;

	double magnitude() const;

	Vector2 normalize() const;

	Vector2 operator+(const Vector2& vec) const;

	Vector2 operator-(const Vector2& vec) const;

	Vector2 operator*(double scalar) const;
};

class Vector3 {
public:
	double x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

	Vector3 cross(const Vector3 &vec) const;

	double dot(const Vector3& vec) const;
	
	double magnitude() const;

	Vector3 normalize() const;

	Vector3 operator+(const Vector3 &vec) const;

	Vector3 operator-(const Vector3& vec) const;

	Vector3 operator*(double scalar) const;

	Point3 operator=(const Point3& other) const;

	Vector2 toXY() const;
	Vector2 toYX() const;

	Vector2 toYZ() const;
	Vector2 toZY() const;

	Vector2 toZX() const;
	Vector2 toXZ() const;
};


#endif /* VECTOR_H */
