#pragma once

#ifndef POINT_H
#define POINT_H

#include "mainHeader.h"
#include "vector.h"

class Point2 {
public:
	float x, y;
	Point2() : x(0), y(0) {}
	Point2(float x, float y) : x(x), y(y) {}
};

class Point3 {
public:
	float x, y, z;
	Point3() : x(0), y(0), z(0) {}
	Point3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator-(const Point3& other) const;
	Vector3 operator+(const Point3& other) const;
};


#endif /* POINT_H */
