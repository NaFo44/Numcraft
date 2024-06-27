#pragma once

#ifndef MOTEUR_GRAPHIQUE_H
#define MOTEUR_GRAPHIQUE_H

#include "lib_math.h"
#include <string>

void toggleWireframeMode();
void clear();
void putPixel(const vec2 &v);
void putTriangle(const Triangle2D &tri);
void putMesh(const Triangle3D triangles[], int numTriangles, const Camera &cam);
extern const int width;
extern const int height;
void addCube(Triangle3D* triangles, int& triangleCount, float x, float y, float z);
void removeCube(Triangle3D* triangles, int& triangleCount, float x, float y, float z);

#endif