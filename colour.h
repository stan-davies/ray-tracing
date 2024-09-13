#ifndef COLOUR_H
#define COLOUR_H

#include "float3.h"

using colour = float3;

void write_colour(std::ostream& out, const colour& pixel_colour) {
    float r = pixel_colour.x();
    float g = pixel_colour.y();
    float b = pixel_colour.z();

    // translate [0,1] to byte range [0,255]
    int rbyte = int(255.999f * r);
    int gbyte = int(255.999f * g);
    int bbyte = int(255.999f * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif