#include "../include/vector.h"
#include "../include/point.h"
#include "../include/angles.h"

float Vector3::dot(const Vector3& vec) const {
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector3 Vector3::cross(const Vector3& vec) const {
    return Vector3(
        y * vec.z - z * vec.y,
        z * vec.x - x * vec.z,
        x * vec.y - y * vec.x
    );
}

float Vector3::magnitude() const {
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

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(
        x * scalar,
        y * scalar,
        z * scalar
    );
}

Vector3 Vector3::normalize() const {
    float mag = magnitude();
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

void movePointForward(Point3& pointPosition, EulerAngles &angles, float distance) {
    if (angles.pitch > M_PI / 2) angles.pitch = M_PI / 2;
    if (angles.pitch < -M_PI / 2) angles.pitch = -M_PI / 2;

    // Calculate the forward direction based on yaw and pitch
    float forward_x = cos(angles.pitch) * sin(angles.yaw);
    float forward_y = sin(angles.pitch);
    float forward_z = cos(angles.pitch) * cos(angles.yaw);

    // Update the point's position by moving it forward
    pointPosition.x += forward_x * distance;
    pointPosition.y += forward_y * distance;
    pointPosition.z += forward_z * distance;
}

void movePointLeft(Point3& pointPosition, EulerAngles &angles, float distance) {

    float forward_x = cos(angles.pitch) * sin(angles.yaw);
    float forward_y = sin(angles.pitch);
    float forward_z = cos(angles.pitch) * cos(angles.yaw);

    float right_x = cos(angles.pitch) * cos(angles.yaw);
    float right_y = 0.0f;
    float right_z = -sin(angles.pitch) * sin(angles.yaw);

    pointPosition.x += right_x * distance;
    pointPosition.y += right_y * distance;
    pointPosition.z += right_z * distance;
}


void movePointBack(Point3& pointPosition, EulerAngles &angles, float distance) {
    movePointForward(pointPosition, angles, -distance);
}

void movePointRight(Point3& pointPosition, EulerAngles &angles, float distance) {
    movePointLeft(pointPosition, angles, -distance);
}