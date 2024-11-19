#include "../include/vector.h"
#include "../include/point.h"
#include "../include/angles.h"

double Vector3::dot(const Vector3& vec) const {
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector3 Vector3::cross(const Vector3& vec) const {
    return Vector3(
        y * vec.z - z * vec.y,
        z * vec.x - x * vec.z,
        x * vec.y - y * vec.x
    );
}

double Vector3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::operator+(const Vector3& vec) const {
    return Vector3(
        x + vec.x,
        y + vec.y,
        z + vec.z
    );
}

Vector3 Vector3::operator-(const Vector3& vec) const {
    return Vector3(
        x - vec.x,
        y - vec.y,
        z - vec.z
    );
}

Vector3 Vector3::operator*(double scalar) const {
    return Vector3(
        x * scalar,
        y * scalar,
        z * scalar
    );
}

Vector3 Vector3::normalize() const {
    double mag = magnitude();
    return (mag > 0) ? Vector3(x / mag, y / mag, z / mag) : Vector3(0, 0, 0);
}

Vector2 Vector3::toXY() const {
    return Vector2(x, y);
}

Vector2 Vector3::toYX() const {
    return Vector2(y, x);
}

Vector2 Vector3::toYZ() const {
    return Vector2(y, z);
}

Vector2 Vector3::toZY() const {
    return Vector2(z, y);
}

Vector2 Vector3::toXZ() const {
    return Vector2(x, z);
}

Vector2 Vector3::toZX() const {
    return Vector2(z, x);
}

Point3 Point3::operator-(const Point3 &other) const {
    return Point3(x - other.x, y - other.y, z - other.z);
}

Point3 Point3::operator+(const Point3& other) const {
    return Point3(x + other.x, y + other.y, z + other.z);
}

Point3 Vector3::operator=(const Point3& other) const {
    return Point3(x, y, z);
}

void movePointForward(Point3& pointPosition, EulerAngles &angles, double distance) {

    // std::cout << "Angles' sine value: " << "<" << sin(angles.pitch) << ", " << sin(angles.yaw) << ", " << sin(angles.roll) << ">\n";

    // Calculate the forward direction based on yaw and pitch
    double forward_x = cos(angles.pitch) * sin(angles.yaw);
    double forward_y = -sin(angles.pitch);
    double forward_z = cos(angles.pitch) * cos(angles.yaw);

    // Update the point's position by moving it forward
    pointPosition.x += forward_x * distance;
    pointPosition.y += forward_y * distance;
    pointPosition.z += forward_z * distance;
}

void movePointRight(Point3& pointPosition, EulerAngles &angles, double distance) {

    double forward_x = cos(angles.pitch) * sin(angles.yaw);
    double forward_y = sin(angles.pitch);
    double forward_z = cos(angles.pitch) * cos(angles.yaw);

    Vector3 rightVect = Vector3(forward_x, forward_y, forward_z).cross(Vector3(0, 1, 0));

    pointPosition.x += rightVect.x * distance;
    pointPosition.y += rightVect.y * distance;
    pointPosition.z += rightVect.z * distance;
}


void movePointBack(Point3& pointPosition, EulerAngles &angles, double distance) {
    movePointForward(pointPosition, angles, -distance);
}

void movePointLeft(Point3& pointPosition, EulerAngles &angles, double distance) {
    movePointRight(pointPosition, angles, -distance);
}