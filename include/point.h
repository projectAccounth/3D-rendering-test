#pragma once

#ifndef POINT_H
#define POINT_H

#include "mainHeader.h"

class Point3 {
public:
	double x, y, z;
	Point3() : x(0), y(0), z(0) {}
	Point3(double x, double y, double z) : x(x), y(y), z(z) {}

	Point3 operator-(const Point3& other) const;
	Point3 operator+(const Point3& other) const;
};

class Point2 {
public:
	double x, y;
	Point2() : x(0), y(0) {}
	Point2(double x, double y) : x(x), y(y) {}
};

#endif /* POINT_H */
