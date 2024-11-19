#include "../include/vector.h"

double Vector2::dot(const Vector2& vec) const {
    return (x * vec.x) + (y * vec.y);
}

double Vector2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::operator+(const Vector2& vec) const {
    return Vector2(
        x + vec.x,
        y + vec.y
    );
}

Vector2 Vector2::operator-(const Vector2& vec) const {
    return Vector2(
        x - vec.x,
        y - vec.y
    );
}

Vector2 Vector2::operator*(double scalar) const {
    return Vector2(
        x * scalar,
        y * scalar
    );
}

Vector2 Vector2::normalize() const {
    double mag = magnitude();
    return (mag > 0) ? Vector2(x / mag, y / mag) : Vector2(0, 0);
}