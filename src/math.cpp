#include "math.h"

#define PI 3.141592653589793238462643383279502884197169399375105

float cos(float x) {
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;

    float term = 1;
    float result = 1;
    for (int i = 1; i <= NUM_TERMS; ++i) {
        term *= -x * x / (2 * i * (2 * i - 1));
        result += term;
    }
    return result;
}

float sin(float x) {
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;

    float term = x;
    float result = x;
    for (int i = 1; i <= NUM_TERMS; ++i) {
        term *= -x * x / (2 * i * (2 * i + 1));
        result += term;
    }
    return result;
}

int round(double number) {
    return static_cast<int>((number < 0.0) ? (number - 0.5) : (number + 0.5));
}

float sqrt(float number) {
    if (number < 0) return -1;
    if (number == 0) return 0;

    float x = number;
    float y = 1.0;
    float epsilon = 0.000001;

    while (x - y > epsilon) {
        x = (x + y) / 2;
        y = number / x;
    }

    return x;
}