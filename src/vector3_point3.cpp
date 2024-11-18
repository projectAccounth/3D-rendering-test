#include "../include/vector.h"
#include "../include/point.h"

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
