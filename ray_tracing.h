#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
// a shared pointer is a pointer to an object where the amount of instances is counted
// this means assignment increases the count, deletion decreases the count
// if the count hits 0, then the pointer is completely deleted
using std::shared_ptr;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415927f;

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.f;
}

#include "colour.h"
#include "ray.h"
#include "float3.h"

#endif