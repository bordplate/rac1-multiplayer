//
// Created by bordplate on 5/25/2025.
//

#ifndef RAC1_MULTIPLAYER_MATH_H
#define RAC1_MULTIPLAYER_MATH_H

#include "types.h"

double sqrt_approx(double number, double tolerance = 1e-10) {
    if (number < 0) {
        return -1; // Return -1 for negative numbers as error indicator
    }
    if (number == 0) {
        return 0; // The square root of 0 is 0
    }

    double estimate = number;
    double diff = tolerance;

    while (diff >= tolerance) {
        double new_estimate = 0.5 * (estimate + number / estimate);
        diff = (estimate > new_estimate) ? estimate - new_estimate : new_estimate - estimate;
        estimate = new_estimate;
    }

    return estimate;
}

float distance(const Vec4& a, const Vec4& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return sqrt_approx(dx * dx + dy * dy + dz * dz);
}

#endif //RAC1_MULTIPLAYER_MATH_H
