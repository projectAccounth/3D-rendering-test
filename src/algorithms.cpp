#include "../include/algorithms.h"

void bresenham(Point2 p0, Point2 p1, std::vector<std::vector<image>>& list) {

	int height = list.size();
	if (height == 0) return;

	int width = list[0].size();

	auto isInsideBounds = [width, height](int x, int y) -> bool {
		return x >= 0 && x < width && y >= 0 && y < height;
		};

	std::cout << "p1: " << p1.x << ' ' << p1.y << '\n';

	// Round the floating-point coordinates to integers for Bresenham
	int x0 = static_cast<int>(std::round(p0.x));
	int y0 = static_cast<int>(std::round(p0.y));
	int x1 = static_cast<int>(std::round(p1.x));
	int y1 = static_cast<int>(std::round(p1.y));

	// std::cout << x0 << ' ' << x1;

	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	if (x0 == x1) sx = 0;
	if (y0 == y1) sy = 0;

	// Calculate the absolute differences in coordinates
	int dx = std::abs(x1 - x0);
	int dy = -std::abs(y1 - y0);

	// Initialize the error term for integer precision
	int error = dx + dy;

	p0 = { static_cast<float>(x0), static_cast<float>(y0) };

	while (true) {
		// Use the integer rounded coordinates to set the image at the pixel location
		if (isInsideBounds(x0, y0)) {
			list[y0][x0].filePath = "./res/imgs/lamp_on.png";
			// std::cout << "Specified location: (" << x0 << " " << y0 << ")\n";
		}

		// std::cout << "Current location: (" << x0 << " " << y0 << ")\n";
		// std::cout << "Current location (1): (" << x1 << " " << y1 << ")\n";

		p0 = { static_cast<float>(x0), static_cast<float>(y0) }; // Update the last rounded point

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

	// Step 4: Projection
	const float EPSILON = 1e-6f;
	projectedVertices.resize(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i) {
		if (std::abs(viewVertices[i].z) > EPSILON) {
			projectedVertices[i].x = (displayPosition.z / viewVertices[i].z) * viewVertices[i].x + displayPosition.x;
			projectedVertices[i].y = (displayPosition.z / viewVertices[i].z) * viewVertices[i].y + displayPosition.y;
		}
		else {
			projectedVertices[i].x = displayPosition.x;
			projectedVertices[i].y = displayPosition.y;
		}
	}
}

void updateAngles(EulerAngles camRotation, Point3 camPos, Point3 displayPos, std::vector<Vector3> d_vs, std::vector<Point3> vertices, std::vector<Point2> projectionPs) {
	if (d_vs.size() != vertices.size() || projectionPs.size() != vertices.size()) {
		std::cerr << "Size mismatch between vectors!" << std::endl;
		return;
	}

	Matrix3d rotationMatrix = EulerToMatrixLH(camRotation);
	const float EPSILON = 1e-6f;

	for (size_t i = 0; i < vertices.size(); ++i) {
		Point3 relativePos = vertices[i] - camPos;
		d_vs[i] = rotationMatrix * relativePos;

		if (std::abs(d_vs[i].z) > EPSILON) { // Valid depth
			projectionPs[i].x = (displayPos.z / d_vs[i].z) * d_vs[i].x + displayPos.x;
			projectionPs[i].y = (displayPos.z / d_vs[i].z) * d_vs[i].y + displayPos.y;
		}
		else {
			// Handle points too close to the camera's plane
			projectionPs[i].x = displayPos.x;
			projectionPs[i].y = displayPos.y;
			std::cout << "Warning: Vertex " << i << " is too close to the camera plane!\n";
		}
	}
}

void resetAllImages(std::vector<std::vector<image>>& images) {
	for (auto& imgs : images) {
		for (auto& img : imgs) {
			img.filePath = "./res/imgs/lamp.png";
		}
	}
}
void updateImage(std::vector<Point3> &vertices, std::vector<Point2> &projectP, imageManager &manager) {
	resetAllImages(manager.images2d);

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