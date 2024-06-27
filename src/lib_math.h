#pragma once

#ifndef LIB_MATH_H
#define LIB_MATH_H

#include <cstdint>

class vec2 {
public:
    float x, y;

    vec2(float x = 0, float y = 0);
    vec2 multiply(float c) const;
    vec2 add(const vec2 &v) const;
    vec2 toScreen() const;
};

class vec3 {
public:
    float x, y, z;

    vec3(float x = 0, float y = 0, float z = 0);
    vec3 multiply(float c) const;
    vec3 add(const vec3 &v) const;
    vec3 minus(const vec3 &v) const;
    vec2 projection() const;
    vec3 rotationX(float pitch) const;
    vec3 rotationY(float yaw) const;

    vec3& operator+=(const vec3& v);
    vec3& operator-=(const vec3& v);

    float dot(const vec3 &v) const;
    vec3 normalize() const;
    vec3 cross(const vec3 &v) const;
};

class Triangle2D {
public:
    vec2 v1, v2, v3;
    uint32_t color;

    Triangle2D(const vec2 &v1, const vec2 &v2, const vec2 &v3, uint32_t color = 0xFFFF);
    Triangle2D toScreen() const;
};

class Triangle3D {
public:
    vec3 v1, v2, v3;
    uint32_t color;

    Triangle3D() : v1(vec3(0, 0, 0)), v2(vec3(0, 0, 0)), v3(vec3(0, 0, 0)), color(0xFFFF) {}
    Triangle3D(const vec3 &v1, const vec3 &v2, const vec3 &v3, uint32_t color = 0xFFFF);

    Triangle2D projection() const;
    Triangle3D translate(const vec3 &v) const;
    Triangle3D rotationX(float pitch) const;
    Triangle3D rotationY(float yaw) const;
};

class Camera {
public:
    vec3 position;
    float pitch;
    float yaw;
    float focalLength;

    Camera(const vec3 &position, float pitch, float yaw, float focalLength = 2);
    vec3 getLookAtDirection() const;
    vec3 getForwardDirection() const;
    vec3 getRightDirection() const;
};

float distanceTriangle(const Triangle3D& triangle, const Camera& cam);

#endif
