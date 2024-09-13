#include <iostream>
#include "colour.h"

int main() {
    const int image_width = 256;
    const int image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y) {
        // \r is rollback, which goes back up a line to output, meaning this will all go over each other, also this stuff won't end up in the file
        std::clog << "\rscanlines remaining: " << (image_height - y) << ' ' << std::flush;
        for (int x = 0; x < image_width; ++x) {
            colour pixel_colour = colour(
                float(x) / (image_width - 1),
                float(y) / (image_height - 1),
                0
            );

            write_colour(std::cout, pixel_colour);
        }
    }

    std::clog << "\rdone                     \n";
}