#pragma once

#ifndef BOXES_H
#define BOXES_H

#include "algorithms.h"

class Box {
public:
	std::vector<Point3> vertices;

	int boxSize;

	Box() : boxSize(1),
			vertices({Point3(1, 1, 1), Point3(1, 1, 0), Point3(0, 1, 1), Point3(0, 1, 0),
					  Point3(1, 0, 1), Point3(1, 0, 0), Point3(0, 0, 1), Point3(0, 0, 0) })
			{}

	Box(Point3 p1, int boxSize) :
		boxSize(boxSize),
		vertices({Point3(p1.x, p1.y, p1.z), Point3(p1.x, p1.y, p1.z - boxSize),
				  Point3(p1.x - boxSize, p1.y, p1.z), Point3(p1.x - boxSize, p1.y, p1.z - boxSize),
				  Point3(p1.x, p1.y - boxSize, p1.z), Point3(p1.x, p1.y - boxSize, p1.z - boxSize),
				  Point3(p1.x - boxSize, p1.y - boxSize, p1.z), Point3(p1.x - boxSize, p1.y - boxSize, p1.z - boxSize) }) {}
};

class BoxManager {
public:
	std::vector<Box> boxes;

	void add(Box box);
};

#endif /* BOXES_H */
