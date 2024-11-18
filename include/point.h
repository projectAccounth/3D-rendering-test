#pragma once

#ifndef POINT_H
#define POINT_H

#include "mainHeader.h"

class Point3 {
public:
	float x, y, z;
	Point3() : x(0), y(0), z(0) {}
	Point3(float x, float y, float z) : x(x), y(y), z(z) {}

	Point3 operator-(const Point3& other) const;
	Point3 operator+(const Point3& other) const;
};

class Point2 {
public:
	float x, y;
	Point2() : x(0), y(0) {}
	Point2(float x, float y) : x(x), y(y) {}
};

#endif /* POINT_H */
