#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include "mainHeader.h"
#include "point.h"

class Vector2 {
public:
	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}

	float dot(const Vector2& vec) const;

	float magnitude() const;

	Vector2 normalize() const;

	Vector2 operator+(const Vector2& vec) const;

	Vector2 operator-(const Vector2& vec) const;

	Vector2 operator*(float scalar) const;
};

class Vector3 {
public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 cross(const Vector3 &vec) const;

	float dot(const Vector3& vec) const;
	
	float magnitude() const;

	Vector3 normalize() const;

	Vector3 operator+(const Vector3 &vec) const;

	Vector3 operator-(const Vector3& vec) const;

	Vector3 operator*(float scalar) const;

	Point3 operator=(const Point3& other) const;

	Vector2 toXY() const;
	Vector2 toYX() const;

	Vector2 toYZ() const;
	Vector2 toZY() const;

	Vector2 toZX() const;
	Vector2 toXZ() const;
};


#endif /* VECTOR_H */
