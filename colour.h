#ifndef COLOUR_H
#define COLOUR_H

#include "double3.h"
// read the note around the ray tracing include in ray_tracing.h
// interestingly, vscode does not require this include to think all is well
// the compiler (and I) disagree with this, as in ray_tracing.h, colour.h is included before interval.h
// perhaps it does not care about order??
#include "interval.h"

using colour = double3;

void write_colour(std::ostream& out, const colour& pixel_colour) {
    double r = pixel_colour.x();
    double g = pixel_colour.y();
    double b = pixel_colour.z();

    // translate [0,1] (clamped to) to byte range [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256.0 * intensity.clamp(r));
    int gbyte = int(256.0 * intensity.clamp(g));
    int bbyte = int(256.0 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif