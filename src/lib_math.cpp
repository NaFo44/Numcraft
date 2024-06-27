#include "lib_math.h"
#include "moteur_graphique.h"
#include "eadkpp.h"
#include "math.h"

inline float fastInverseSqrt(float number) {
    union {
        float f;
        uint32_t i;
    } conv = { number };

    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5f - 0.5f * number * conv.f * conv.f;
    return conv.f;
}

vec2::vec2(float x, float y) : x(x), y(y) {}

vec2 vec2::multiply(float c) const {
    return vec2(x * c, y * c);
}

vec2 vec2::add(const vec2 &v) const {
    return vec2(x + v.x, y + v.y);
}

vec2 vec2::toScreen() const {
    return vec2((x + 1) * (width / 2), (-y + 1) * (height / 2));
}

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

vec3 vec3::multiply(float c) const {
    return vec3(x * c, y * c, z * c);
}

vec3 vec3::add(const vec3 &v) const {
    return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::minus(const vec3 &v) const {
    return vec3(x - v.x, y - v.y, z - v.z);
}

vec2 vec3::projection() const {
    return vec2(x / z, y / z);
}

vec3 vec3::rotationX(float pitch) const {
    float cos_pitch = cos(pitch);
    float sin_pitch = sin(pitch);
    float y1 = cos_pitch * y - sin_pitch * z;
    float z1 = sin_pitch * y + cos_pitch * z;
    return vec3(x, y1, z1);
}

vec3 vec3::rotationY(float yaw) const {
    float cos_yaw = cos(yaw);
    float sin_yaw = sin(yaw);
    float x1 = cos_yaw * x + sin_yaw * z;
    float z1 = -sin_yaw * x + cos_yaw * z;
    return vec3(x1, y, z1);
}

vec3& vec3::operator+=(const vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vec3& vec3::operator-=(const vec3 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

float vec3::dot(const vec3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

vec3 vec3::normalize() const {
    float length = x * x + y * y + z * z;
    if (length > 0) {
        float invLength = fastInverseSqrt(length);
        return vec3(x * invLength, y * invLength, z * invLength);
    }
    return *this;
}

vec3 vec3::cross(const vec3 &v) const {
    return vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

Triangle2D::Triangle2D(const vec2 &v1, const vec2 &v2, const vec2 &v3, uint32_t color) : v1(v1), v2(v2), v3(v3), color(color) {}

Triangle2D Triangle2D::toScreen() const {
    return Triangle2D(v1.toScreen(), v2.toScreen(), v3.toScreen(), color);
}

Triangle3D::Triangle3D(const vec3 &v1, const vec3 &v2, const vec3 &v3, uint32_t color) : v1(v1), v2(v2), v3(v3), color(color) {}

Triangle2D Triangle3D::projection() const {
    return Triangle2D(v1.projection(), v2.projection(), v3.projection(), color);
}

Triangle3D Triangle3D::translate(const vec3 &v) const {
    return Triangle3D(v1.add(v), v2.add(v), v3.add(v), color);
}

Triangle3D Triangle3D::rotationX(float pitch) const {
    return Triangle3D(v1.rotationX(pitch), v2.rotationX(pitch), v3.rotationX(pitch), color);
}

Triangle3D Triangle3D::rotationY(float yaw) const {
    return Triangle3D(v1.rotationY(yaw), v2.rotationY(yaw), v3.rotationY(yaw), color);
}

Camera::Camera(const vec3 &position, float pitch, float yaw, float focalLength)
    : position(position), pitch(pitch), yaw(yaw), focalLength(focalLength) {}

vec3 Camera::getLookAtDirection() const {
    return vec3(-sin(yaw) * cos(pitch), sin(pitch), cos(yaw) * cos(pitch));
}

vec3 Camera::getForwardDirection() const {
    return vec3(-sin(yaw), 0, cos(yaw));
}

vec3 Camera::getRightDirection() const {
    return vec3(cos(yaw), 0, sin(yaw));
}

float distanceTriangle(const Triangle3D& triangle, const Camera& cam) {
    vec3 centroid = (triangle.v1.add(triangle.v2).add(triangle.v3)).multiply(1.0f / 3.0f);
    vec3 toCamera = centroid.minus(cam.position);
    return toCamera.dot(toCamera);
}