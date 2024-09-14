#include "ray_tracing.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "interval.h"

colour ray_colour(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + colour(1, 1, 1));
    }

    // # background
    vec3 unit_direction = unit(r.direction());
    // normalised y is such that -1 < y < 1 so we add one and half it to yield a value of a such that 0 < a < 1 which we use for lerping
    double a = 0.5 * (unit_direction.y() + 1.0);
    // lerp between white and blue, using the precalculated value of a
    return ((1.0 - a) * colour(1.0, 1.0, 1.0)) + (a * colour(0.5, 0.7, 1.0));
}

int main() {
    // this is a target, the actual aspect ratio is likely to vary slightly as the image dimensions must be real valued
    const double aspect_ratio = 16.0 / 9.0;

    // # image
    const int image_width  = 400;
    // we could add some way of ensuring that this is greater than 1, but I haven't done that because it is, i promise
    const int image_height = int(image_width / aspect_ratio);

    // # world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));

    // # camera
    // camera centre is 1 unit from the viewport
    const double focal_length    = 1.0;
    const double viewport_height = 2.0;
    const double viewport_width  = viewport_height * (double(image_width) / image_height);
    // we are using right handed coordinates
    const point3 camera_centre  = { 0, 0, 0 };

    // vectors across the horizontal and down the vertical viewport edges
    const double3 viewport_u = double3(viewport_width, 0, 0);
    const double3 viewport_v = double3(0, -viewport_height, 0);

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

            colour pixel_colour = ray_colour(r, world);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::clog << "\rdone                     \n";
}