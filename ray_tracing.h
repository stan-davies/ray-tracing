#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <cmath>
#include <cstdlib>
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

// returns random real in [0,1)
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

// returns random real in [min,max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "double3.h"

#endif