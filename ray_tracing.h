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

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include "colour.h"
#include "ray.h"
#include "double3.h"

#endif