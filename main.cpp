#include <iostream>
#include "float3.h"
#include "colour.h"
#include "ray.h"

colour ray_colour(const ray& r) {
    vec3 unit_direction = unit(r.direction());
    float a = 0.5f * (unit_direction.y() + 1.f);
    // lerp between white and blue
    return ((1.f - a) * colour(1.f, 1.f, 1.f)) + (a * colour(0.5f, 0.7f, 1.f));
}

int main() {
    // this is a target, the actual aspect ratio is likely to vary slightly as the image dimensions must be real valued
    const float aspect_ratio = 16.f / 9.f;

    // # image
    const int image_width  = 400;
    // we could add some way of ensuring that this is greater than 1, but I haven't done that because it is, i promise
    const int image_height = int(image_width / aspect_ratio);

    // # camera
    // camera centre is 1 unit from the viewport
    const float focal_length    = 1.f;
    const float viewport_height = 2.f;
    const float viewport_width  = viewport_height * (float(image_width)/image_height);
    // we are using left handed coordinates (online thing says right handed but I dispute this)
    const point3 camera_centre  = { 0, 0, 0 };

    // vectors across the horizontal and down the vertical viewport edges
    const float3 viewport_u = float3(viewport_width, 0, 0);
    const float3 viewport_v = float3(0, -viewport_height, 0);

    // pixel to pixel vector deltas
    const vec3 pixel_du = viewport_u / image_width;
    const vec3 pixel_dv = viewport_v / image_height;

    // upper left pixel location
    const point3 viewport_upper_left = camera_centre - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    // centre of the pixel at (0, 0)
    const point3 pixel00_point = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);

    // ---

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y) {
        // \r is rollback, which goes back up a line to output, meaning this will all go over each other, also this stuff won't end up in the file
        std::clog << "\rscanlines remaining: " << (image_height - y) << ' ' << std::flush;
        for (int x = 0; x < image_width; ++x) {
            point3 pixel_centre = pixel00_point + (x * pixel_du) + (y * pixel_dv);
            vec3 ray_direction = pixel_centre - camera_centre;

            ray r(camera_centre, ray_direction);

            colour pixel_colour = ray_colour(r);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::clog << "\rdone                     \n";
}