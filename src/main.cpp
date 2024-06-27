#include "moteur_graphique.h"
#include "lib_math.h"
#include "eadkpp.h"
#include "palette.h"
#include <cstdio> 
#include <cstring>

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "NumCraft";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

const int mapWidth = 4;
const int mapHeight = 4;
const int maxTriangles = mapWidth * mapHeight * 12;
Triangle3D triangles[maxTriangles];
int triangleCount = 0;

Camera cam(vec3(0, 0, 3), 0, 0);

void inputs() {
    EADK::Keyboard::State currentState = EADK::Keyboard::scan();
    if (currentState.keyDown(EADK::Keyboard::Key::Up) && cam.pitch < 1.57) {
        cam.pitch += 0.1;
    } else if (currentState.keyDown(EADK::Keyboard::Key::Down) && cam.pitch > -1.57) {
        cam.pitch -= 0.1;
    } else if (currentState.keyDown(EADK::Keyboard::Key::Left)) {
        cam.yaw += 0.1;
    } else if (currentState.keyDown(EADK::Keyboard::Key::Right)) {
        cam.yaw -= 0.1;
    } else if (currentState.keyDown(EADK::Keyboard::Key::Plus)) {
        cam.position += cam.getForwardDirection().multiply(0.1);
    } else if (currentState.keyDown(EADK::Keyboard::Key::Minus)) {
        cam.position -= cam.getForwardDirection().multiply(0.1);
    } else if (currentState.keyDown(EADK::Keyboard::Key::Four)) {
        cam.position -= cam.getRightDirection().multiply(0.1);
    } else if (currentState.keyDown(EADK::Keyboard::Key::Six)) {
        cam.position += cam.getRightDirection().multiply(0.1);
    } else if (currentState.keyDown(EADK::Keyboard::Key::Two)) {
        cam.position.y -= 0.1;
    } else if (currentState.keyDown(EADK::Keyboard::Key::Eight)) {
        cam.position.y += 0.1;
    }
    //  else if (currentState.keyDown(EADK::Keyboard::Key::One)) {
    //     addCube(triangles, triangleCount, cam.position.x, cam.position.y, cam.position.z);
    // } else if (currentState.keyDown(EADK::Keyboard::Key::Three)) {
    //     removeCube(triangles, triangleCount, cam.position.x, cam.position.y, cam.position.z);
    // }
}

void generateTerrain(Triangle3D* triangles, int& triangleCount, int width, int height, int n) {
    int size = width * height;
    int m[size];
    int m2[size];

    for (int i = 0; i < size; i++) {
        m[i] = EADK::random() % 256;
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < size; i++) {
            int a[5];
            int count = 0;
            
            a[count++] = m[i];
            
            if (i % width > 0) a[count++] = m[i - 1];
            if (i % width < width - 1) a[count++] = m[i + 1];
            if (i / width > 0) a[count++] = m[i - width];
            if (i / width < height - 1) a[count++] = m[i + width];
            
            int sum = 0;
            for (int k = 0; k < count; k++) {
                sum += a[k];
            }
            m2[i] = sum / count;
        }
        
        for (int i = 0; i < size; i++) {
            m[i] = m2[i];
        }
    }

    float offsetX = static_cast<float>(width) / 2.0f;
    float offsetZ = static_cast<float>(height) / 2.0f - 3;

    for (int i = 0; i < size; i++) {
        float terrainHeight = static_cast<float>(m[i]) / 255.0f * 10.0f;
        int cubeHeight = static_cast<int>(terrainHeight);
        float x = static_cast<float>(i % width) - offsetX;
        float z = static_cast<float>(i / width) - offsetZ;
        float y = 7;
        
        addCube(triangles, triangleCount, x, cubeHeight-y, z);
    }
}

int main() {
    generateTerrain(triangles, triangleCount, mapWidth, mapHeight, 3);

    while (true) {
        inputs();
        putMesh(triangles, triangleCount, cam);
    }

    return 0;
}