#include "moteur_graphique.h"
#include "lib_math.h"
#include "eadkpp.h"
#include "palette.h"
#include <algorithm>
#include <cstdint>
#include "math.h"

constexpr int width = 320;
constexpr int height = 240;
uint16_t lineBuffer[width];

template<typename T>
constexpr const T& min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template<typename T>
constexpr const T& max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

void renderLineBuffer(int y) {
    EADK::Display::pushRect(EADK::Rect(0, y, width, 1), reinterpret_cast<const EADK::Color*>(lineBuffer));
}

void print(const char * txt) {
    EADK::Display::drawString(txt, EADK::Point(0,0), true, Black, White);
}

void putPixel(const vec2 &v, uint16_t color) {
    int px = round(v.x);
    int py = round(v.y);
    if (0 <= px && px < width && 0 <= py && py < height) {
        lineBuffer[px] = color;
    }
}

vec3 calculateNormal(const Triangle3D &triangle) {
    vec3 u = triangle.v2.minus(triangle.v1);
    vec3 v = triangle.v3.minus(triangle.v1);
    return u.cross(v).normalize();
}

bool isTriangleVisible(const Triangle3D &checkTriangle, const Camera &cam) {
    vec3 normal = calculateNormal(checkTriangle);
    vec3 viewDir = (checkTriangle.v1.minus(cam.position)).normalize();
    return normal.dot(viewDir) > 0;
}

bool isTriangleInScreen(const Triangle2D& tri) {
    auto inBounds = [](const vec2& v) {
        return v.x >= 0 && v.x < width && v.y >= 0 && v.y < height;
    };
    return inBounds(tri.v1) || inBounds(tri.v2) || inBounds(tri.v3);
}

void putTriangle(const Triangle2D &tri, int y) {
        // Remplissage des triangles dans le mode normal
        auto eq = [](const vec2 &p, const vec2 &a, const vec2 &b) {
            return (a.x - p.x) * (b.y - p.y) - (a.y - p.y) * (b.x - p.x);
        };

        int xmin = max(0, static_cast<int>(min(tri.v1.x, min(tri.v2.x, tri.v3.x))));
        int xmax = min(width, static_cast<int>(max(tri.v1.x, max(tri.v2.x, tri.v3.x))) + 1);
        int ymin = max(0, static_cast<int>(min(tri.v1.y, min(tri.v2.y, tri.v3.y))));
        int ymax = min(height, static_cast<int>(max(tri.v1.y, max(tri.v2.y, tri.v3.y))) + 1);

        if (y < ymin || y >= ymax) {
            return;
        }

        for (int x = xmin; x < xmax; ++x) {
            vec2 pos(x, y);
            int w1 = eq(pos, tri.v3, tri.v1);
            int w2 = eq(pos, tri.v1, tri.v2);
            int w3 = eq(pos, tri.v2, tri.v3); 
            if ((w1 >= 0 && w2 >= 0 && w3 >= 0) || (-w1 >= 0 && -w2 >= 0 && -w3 >= 0)) {
                lineBuffer[x] = tri.color;
            }
        }
}

void sortTrianglesByDistance(Triangle3D* triangles, float* distances, int count) {
    for (int i = 1; i < count; i++) {
        Triangle3D key = triangles[i];
        float keyDistance = distances[i];
        int j = i - 1;
        while (j >= 0 && distances[j] < keyDistance) {
            triangles[j + 1] = triangles[j];
            distances[j + 1] = distances[j];
            j = j - 1;
        }
        triangles[j + 1] = key;
        distances[j + 1] = keyDistance;
    }
}

void putMesh(const Triangle3D triangles[], int numTriangles, const Camera &cam) {
    float distances[numTriangles];
    Triangle3D sortedTriangles[numTriangles];
    
    // Copier les triangles dans un tableau trié
    for (int i = 0; i < numTriangles; i++) {
        sortedTriangles[i] = triangles[i];
        distances[i] = distanceTriangle(triangles[i], cam);
    }

    sortTrianglesByDistance(sortedTriangles, distances, numTriangles);

    for (int y = 0; y < height; ++y) {
        std::fill_n(lineBuffer, width, sky); // Couleur par défaut du buffer
        for (int i = 0; i < numTriangles; ++i) {
            const Triangle3D &triangle = sortedTriangles[i];
            if (isTriangleVisible(triangle, cam)) {
                Triangle2D transformed = triangle.translate(cam.position.multiply(-1))
                                              .rotationY(cam.yaw)
                                              .rotationX(cam.pitch)
                                              .projection()
                                              .toScreen();
                if (isTriangleInScreen(transformed)) {
                    putTriangle(transformed, y);
                }
            }
        }
        renderLineBuffer(y);
    }
}

EADK::Color terre[]={land, land1, grass, grass1};
EADK::Color sable[]={sand, sand1, sand, sand1};
EADK::Color eau[]={water, water1, water, water1};

void addCube(Triangle3D* triangles, int& triangleCount, float x, float y, float z) {
    // Définir les textures en fonction de la hauteur du cube
    EADK::Color* selectedTexture;

    if (y+7 <= 3) {
        selectedTexture = eau;
    } else if (y+7 <= 4) {
        selectedTexture = sable;
    } else {
        selectedTexture = terre;
    }

    // Vertices du cube
    vec3 v1(x - 0.5f, y - 0.5f, z - 0.5f);
    vec3 v2(x + 0.5f, y - 0.5f, z - 0.5f);
    vec3 v3(x + 0.5f, y + 0.5f, z - 0.5f);
    vec3 v4(x - 0.5f, y + 0.5f, z - 0.5f);
    vec3 v5(x - 0.5f, y - 0.5f, z + 0.5f);
    vec3 v6(x + 0.5f, y - 0.5f, z + 0.5f);
    vec3 v7(x + 0.5f, y + 0.5f, z + 0.5f);
    vec3 v8(x - 0.5f, y + 0.5f, z + 0.5f);

    // Face avant
    triangles[triangleCount++] = Triangle3D(v1, v2, v3, selectedTexture[0]);
    triangles[triangleCount++] = Triangle3D(v1, v3, v4, selectedTexture[1]);

    // Face arrière
    triangles[triangleCount++] = Triangle3D(v5, v7, v6, selectedTexture[0]);
    triangles[triangleCount++] = Triangle3D(v5, v8, v7, selectedTexture[1]);

    // Face de gauche
    triangles[triangleCount++] = Triangle3D(v1, v8, v5, selectedTexture[0]);
    triangles[triangleCount++] = Triangle3D(v1, v4, v8, selectedTexture[1]);

    // Face de droite
    triangles[triangleCount++] = Triangle3D(v2, v6, v7, selectedTexture[0]);
    triangles[triangleCount++] = Triangle3D(v2, v7, v3, selectedTexture[1]);

    // Face du haut
    triangles[triangleCount++] = Triangle3D(v4, v3, v7, selectedTexture[2]);
    triangles[triangleCount++] = Triangle3D(v4, v7, v8, selectedTexture[3]);

    // Face du bas
    triangles[triangleCount++] = Triangle3D(v1, v6, v2, selectedTexture[0]);
    triangles[triangleCount++] = Triangle3D(v1, v5, v6, selectedTexture[1]);
}

void removeCube(Triangle3D* triangles, int& triangleCount, float x, float y, float z) {
    // Trouver les triangles correspondant au cube à la position (x, y, z) et les supprimer
    for (int i = 0; i < triangleCount; i += 12) {
        bool match = true;
        for (int j = 0; j < 12; ++j) {
            if (triangles[i + j].v1.x < x - 0.5f || triangles[i + j].v1.x > x + 0.5f ||
                triangles[i + j].v1.y < y - 0.5f || triangles[i + j].v1.y > y + 0.5f ||
                triangles[i + j].v1.z < z - 0.5f || triangles[i + j].v1.z > z + 0.5f) {
                match = false;
                break;
            }
        }
        if (match) {
            for (int j = i; j < triangleCount - 12; ++j) {
                triangles[j] = triangles[j + 12];
            }
            triangleCount -= 12;
            return;
        }
    }
}
