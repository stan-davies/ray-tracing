#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

// note: camera uses right hand coordinates
class camera {
    public:
        // this is a target, the actual aspect ratio is likely to vary slightly as the image dimensions must be real valued
        double aspect_ratio = 16.0 / 9.0;
        int image_width  = 400;

        void render(const hittable& world) {
            initialise();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int y = 0; y < image_height; ++y) {
                // \r is rollback, which goes back up a line to output, meaning this will all go over each other, also this stuff won't end up in the file
                std::clog << "\rlines remaining: " << (image_height - y) << ' ' << std::flush;
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

    private:
        int      image_height;
        point3   camera_centre;
        double3  pixel_du;
        double3  pixel_dv;
        point3   pixel00_point;


        void initialise() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            camera_centre          = { 0.0, 0.0, 0.0 };
            double focal_length    = 1.0;
            double viewport_height = 2.0;
            double viewport_width  = viewport_height * (double(image_width) / image_height);

            // vectors across the horizontal and down the vertical viewport edges
            double3 viewport_u = double3(viewport_width, 0, 0);
            double3 viewport_v = double3(0, -viewport_height, 0);

            // pixel to pixel vector deltas
            pixel_du = viewport_u / image_width;
            pixel_dv = viewport_v / image_height;

            // upper left pixel location
            const point3 viewport_upper_left = camera_centre - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            // centre of the pixel at (0, 0)
            pixel00_point = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);
        }

        colour ray_colour(const ray& r, const hittable& world) const {
            hit_record rec;

            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + colour(1.0, 1.0, 1.0));
            }

            // # background
            vec3 unit_direction = unit(r.direction());
            // normalised y is such that -1 < y < 1 so we add one and half it to yield a value of a such that 0 < a < 1 which we use for lerping
            double a = 0.5 * (unit_direction.y() + 1.0);
            // lerp between white and blue, using the precalculated value of a
            return ((1.0 - a) * colour(1.0, 1.0, 1.0)) + (a * colour(0.5, 0.7, 1.0));
        }
};

#endif