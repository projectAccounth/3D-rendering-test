#pragma once

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "mainHeader.h"
#include "image.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "angles.h"

void bresenham(Point2 p0, Point2 p1, std::vector<std::vector<image>>& list);

void transformObject(
	const EulerAngles& objectRotationAngles, Point3 objectWorldPos,
	const EulerAngles& cameraRotationAngles, Point3 cameraPosition,
	Point3 displayPosition,
	const std::vector<Point3>& localVertices,
	std::vector<Point2>& projectedVertices
);

void updateAngles(EulerAngles camRotation, Point3 camPos, Point3 displayPos, std::vector<Vector3> d_vs, std::vector<Point3> vertices, std::vector<Point2> projectionPs);

void resetAllImages(std::vector<std::vector<image>>& images);

void updateImage(std::vector<Point3> &vertices, std::vector<Point2> &projectP, imageManager &manager);

Point3 calculateCentroid(const std::vector<Point3>& vertices);

#endif /* ALGORITHMS_H */
