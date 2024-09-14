#ifndef COLOUR_H
#define COLOUR_H

#include "double3.h"

using colour = double3;

void write_colour(std::ostream& out, const colour& pixel_colour) {
    double r = pixel_colour.x();
    double g = pixel_colour.y();
    double b = pixel_colour.z();

    // translate [0,1] to byte range [0,255]
    int rbyte = int(255.999f * r);
    int gbyte = int(255.999f * g);
    int bbyte = int(255.999f * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif