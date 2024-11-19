#include "../include/algorithms.h"
#include "../include/mainHeader.h"

void bresenham(Point2 p0, Point2 p1, std::vector<std::vector<image>>& list) {

	int height = list.size();
	if (height == 0) return;

	int width = list[0].size();

	auto isInsideBounds = [width, height](int x, int y) -> bool {
		return x >= 0 && x < width && y >= 0 && y < height;
	};

	if (std::isnan(p0.x) || std::isnan(p0.y) || std::isnan(p1.x) || std::isnan(p1.y)) {
		// std::cout << "Warning: Line endpoints contain invalid vertices. Skipping line.\n";
		return;
	}


	// Round the doubleing-point coordinates to integers for Bresenham
	int x0 = static_cast<int>(std::round(p0.x));
	int y0 = static_cast<int>(std::round(p0.y));
	int x1 = static_cast<int>(std::round(p1.x));
	int y1 = static_cast<int>(std::round(p1.y));

	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	if (x0 == x1) sx = 0;
	if (y0 == y1) sy = 0;

	// Calculate the absolute differences in coordinates
	int dx = std::abs(x1 - x0);
	int dy = -std::abs(y1 - y0);

	// Initialize the error term for integer precision
	int error = dx + dy;

	p0 = { static_cast<double>(x0), static_cast<double>(y0) };

	while (true) {
		// Use the integer rounded coordinates to set the image at the pixel location
		if (isInsideBounds(x0, y0)) {
			list[y0][x0].filePath = "./res/imgs/orsh_0.png";
		}

		p0 = { static_cast<double>(x0), static_cast<double>(y0) }; // Update the last rounded point

		if (x0 == x1 && y0 == y1) return;

		int e2 = 2 * error;

		if (e2 >= dy) {
			error += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			error += dx;
			y0 += sy;
		}
	}
}

void transformObject(
	const EulerAngles& objectRotationAngles, Point3 objectWorldPos,
	const EulerAngles& cameraRotationAngles, Point3 cameraPosition,
	Point3 displayPosition,
	const std::vector<Point3>& localVertices,
	std::vector<Point2>& projectedVertices
) {
	size_t vertexCount = localVertices.size();
	std::vector<Point3> worldVertices(vertexCount);
	std::vector<Point3> viewVertices(vertexCount);

	// Step 1: Object Local Transformation
	Matrix3d objectRotation = EulerToMatrixLH(objectRotationAngles);
	for (size_t i = 0; i < vertexCount; ++i) {
		worldVertices[i] = objectRotation * localVertices[i];
	}

	// Step 2: World Transformation
	for (size_t i = 0; i < vertexCount; ++i) {
		worldVertices[i] = worldVertices[i] + objectWorldPos;
	}

	// Step 3: Camera Transformation
	Matrix3d cameraRotation = EulerToMatrixLH(cameraRotationAngles);
	for (size_t i = 0; i < vertexCount; ++i) {
		viewVertices[i] = cameraRotation * (worldVertices[i] - cameraPosition);
	}

	// Step 4: Projection with Clipping
	const double nearPlane = 0.1f; // Minimum Z-distance for rendering
	const double EPSILON = 1e-6f;

	projectedVertices.resize(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i) {
		if (viewVertices[i].z < nearPlane) {
			// Mark invalid points
			projectedVertices[i] = { std::nanf(""), std::nanf("") };
			continue;
		}

		if (std::abs(viewVertices[i].z) > EPSILON) {
			projectedVertices[i].x = (displayPosition.z / viewVertices[i].z) * viewVertices[i].x + displayPosition.x;
			projectedVertices[i].y = (displayPosition.z / viewVertices[i].z) * viewVertices[i].y + displayPosition.y;
		}
		else {
			projectedVertices[i].x = displayPosition.x;
			projectedVertices[i].y = displayPosition.y;
		}
	}
	displayPosition = Point3(cameraPosition.x, cameraPosition.y, cameraPosition.z + 10);
}

void updateAngles(EulerAngles camRotation, Point3& camPos, Point3& displayPos,
	std::vector<Vector3> d_vs, std::vector<Point3> vertices,
	std::vector<Point2> projectionPs) {
	const double nearPlane = 0.1f;
	const double farPlane = 100.0f;
	const double EPSILON = 1e-6f;

	if (d_vs.size() != vertices.size() || projectionPs.size() != vertices.size()) {
		// std::cerr << "Size mismatch between vectors!" << std::endl;
		return;
	}

	Matrix3d rotationMatrix = EulerToMatrixLH(camRotation);

	for (size_t i = 0; i < vertices.size(); ++i) {
		Point3 relativePos = vertices[i] - camPos;
		d_vs[i] = rotationMatrix * relativePos;

		// Skip vertices outside the frustum
		if (d_vs[i].z < nearPlane || d_vs[i].z > farPlane) {
			projectionPs[i] = { std::nanf(""), std::nanf("") };
			continue;
		}

		if (std::abs(d_vs[i].z) > EPSILON) { // Valid depth
			projectionPs[i].x = (displayPos.z / d_vs[i].z) * d_vs[i].x + displayPos.x;
			projectionPs[i].y = (displayPos.z / d_vs[i].z) * d_vs[i].y + displayPos.y;
		}
		else {
			projectionPs[i] = { std::nanf(""), std::nanf("") }; // Invalid projection
		}
	}
	displayPos = Point3(camPos.x, camPos.y, camPos.z + 10);
}

void resetAllImages(std::vector<std::vector<image>>& images) {
	for (auto& imgs : images) {
		for (auto& img : imgs) {
			img.filePath = "./res/imgs/orsh_1.png";
		}
	}
}
bool clipLine(Point3& p0, Point3& p1, double nearPlane) {
	bool p0InFront = p0.z >= nearPlane;
	bool p1InFront = p1.z >= nearPlane;

	// If both points are behind the near plane, the line is entirely clipped
	if (!p0InFront && !p1InFront) return false;

	// If one point is behind the near plane, calculate the intersection
	if (p0InFront != p1InFront) {
		double t = (nearPlane - p0.z) / (p1.z - p0.z);
		if (!p0InFront) {
			p0.x += t * (p1.x - p0.x);
			p0.y += t * (p1.y - p0.y);
			p0.z = nearPlane;
		}
		else {
			p1.x += t * (p0.x - p1.x); // Corrected to use p1's original x
			p1.y += t * (p0.y - p1.y); // Corrected to use p1's original y
			p1.z = nearPlane;
		}
	}

	return true;
}

void updateImage1(std::vector<Point3>& vertices, std::vector<Point2>& projectP, imageManager& manager, double nearPlane, Point3 displayPosition) {
	for (size_t i = 0; i < projectP.size(); i++) {
		for (size_t j = 0; j < projectP.size(); j++) {
			if (i == j) continue;

			// Adjacency check in 3D
			bool isAdjacent3D = false;
			const Point3& pt0_3D = vertices[i];
			const Point3& pt1_3D = vertices[j];

			if (pt0_3D.y == pt1_3D.y && pt0_3D.z == pt1_3D.z) {
				isAdjacent3D = true;
			}
			else if (pt0_3D.x == pt1_3D.x && pt0_3D.z == pt1_3D.z) {
				isAdjacent3D = true;
			}
			else if (pt0_3D.x == pt1_3D.x && pt0_3D.y == pt1_3D.y) {
				isAdjacent3D = true;
			}

			if (!isAdjacent3D) continue;

			// Clip the line against the near plane
			Point3 clippedP0 = pt0_3D;
			Point3 clippedP1 = pt1_3D;
			if (!clipLine(clippedP0, clippedP1, nearPlane)) {
				continue; // Skip lines fully behind the near plane
			}

			// Check for valid depth after clipping
			if (clippedP0.z <= 0 || clippedP1.z <= 0) {
				std::cerr << "Invalid depth: line skipped.\n";
				continue;
			}

			// Project clipped points to 2D
			Point2 projectedP0, projectedP1;
			projectedP0.x = (displayPosition.z / clippedP0.z) * clippedP0.x + displayPosition.x;
			projectedP0.y = (displayPosition.z / clippedP0.z) * clippedP0.y + displayPosition.y;

			projectedP1.x = (displayPosition.z / clippedP1.z) * clippedP1.x + displayPosition.x;
			projectedP1.y = (displayPosition.z / clippedP1.z) * clippedP1.y + displayPosition.y;

			// Debug output for projected points
			std::cout << "Projected P0: (" << projectedP0.x << ", " << projectedP0.y << ")\n";
			std::cout << "Projected P1: (" << projectedP1.x << ", " << projectedP1.y << ")\n";

			// Draw the line using Bresenham
			bresenham(projectedP0, projectedP1, manager.images2d);
		}
	}
}

void updateImage(std::vector<Point3>& vertices, std::vector<Point2>& projectP, imageManager& manager, double nearPlane, Point3 displayPosition) {
	for (int i = 0; i < projectP.size(); i++) {
		for (int j = 0; j < projectP.size(); j++) {
			if (i == j) continue;

			bool isAdjacent3D = false;
			const Point3& pt0_3D = vertices[i];
			const Point3& pt1_3D = vertices[j];
			if (pt0_3D.y == pt1_3D.y && pt0_3D.z == pt1_3D.z) {
				isAdjacent3D = true;
			}
			else if (pt0_3D.x == pt1_3D.x && pt0_3D.z == pt1_3D.z) {
				isAdjacent3D = true;
			}
			else if (pt0_3D.x == pt1_3D.x && pt0_3D.y == pt1_3D.y) {
				isAdjacent3D = true;
			}

			if (!isAdjacent3D) continue;
			bresenham(projectP[i], projectP[j], manager.images2d);
		}
	}
}


Point3 calculateCentroid(const std::vector<Point3>& vertices) {
	Point3 centroid{ 0.0f, 0.0f, 0.0f }; // Initialize to (0, 0, 0)
	for (const auto& vertex : vertices) {
		centroid.x += vertex.x;
		centroid.y += vertex.y;
		centroid.z += vertex.z;
	}

	size_t vertexCount = vertices.size();
	if (vertexCount > 0) {
		centroid.x /= vertexCount;
		centroid.y /= vertexCount;
		centroid.z /= vertexCount;
	}

	return centroid;
}

void updateAll(
	const EulerAngles& objectRotationAngles, Point3 objectWorldPos,
	const EulerAngles& cameraRotationAngles, Point3 &cameraPosition,
	Point3 &displayPosition,
	std::vector<Point3>& localVertices,
	std::vector<Point2>& projectedVertices,
	std::vector<Vector3>& d_vs,
	imageManager &manager
) {

	updateAngles(cameraRotationAngles, cameraPosition, displayPosition, d_vs, localVertices, projectedVertices);
	transformObject(objectRotationAngles, objectWorldPos, cameraRotationAngles, cameraPosition, displayPosition, localVertices, projectedVertices);
	updateImage(localVertices, projectedVertices, manager, nearPlane, displayPosition);
}