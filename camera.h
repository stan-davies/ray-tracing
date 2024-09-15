#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

// note: camera uses right hand coordinates
class camera {
    public:
        double  aspect_ratio       = 1.0;           // this is a target, the actual aspect ratio is likely to vary slightly as the image dimensions must be real valued
        int     image_width        = 100;
        int     samples_per_pixel  = 10;            // random samples for each pixel, this is the basic idea of anti-aliasing
        int     max_bounces        = 10;            // maximum number of ray bounces in scene

        double  vfov      = 90;                     // vertical FOV
        point3  lookfrom  = point3(0.0, 0.0,  0.0); // point camera is looking from
        point3  lookat    = point3(0.0, 0.0, -1.0); // point camera is looking at
        vec3    vup       = vec3(0.0, 1.0, 0.0);    // camera-relative "up" direction

        // defocus (or defocus blur) is a ray-tracey way of saying depth of field
        double defocus_angle  = 0;    // variation angle of rays through each pixel
        double focus_distance = 10;   // distance from lookfrom to plane of perfect focus

        void render(const hittable& world) {
            initialise();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int y = 0; y < image_height; ++y) {
                // \r is rollback, which goes back up a line to output, meaning this will all go over each other, also this stuff won't end up in the file
                std::clog << "\rlines remaining: " << (image_height - y) << ' ' << std::flush;
                for (int x = 0; x < image_width; ++x) {
                    colour pixel_colour(0.0, 0.0, 0.0);
                    for (int sample = 0; sample < samples_per_pixel; ++sample) {
                        ray r = get_ray(x, y);
                        pixel_colour += ray_colour(r, max_bounces, world);
                    }

                    write_colour(std::cout, pixel_samples_scale * pixel_colour);
                }
            }

            std::clog << "\rdone                     \n";
        }

    private:
        int      image_height;
        double   pixel_samples_scale;   // colour scale factor for averging sum of pixel samples
        point3   camera_centre;
        double3  pixel_du;              // pixel to pixel vector deltas
        double3  pixel_dv;              //             "
        point3   pixel00_point;         // centre of the pixel at (0, 0)
        vec3     u, v, w;               // camera frame basis vectors
        vec3     defocus_disc_u;        // defocus disc horizontal radius
        vec3     defocus_disc_v;        // defocus disc vertical radius 


        void initialise() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            camera_centre = lookfrom;

            double theta = degrees_to_radians(vfov);
            double h = std::tan(theta / 2);
            double viewport_height = 2.0 * h * focus_distance;
            double viewport_width = viewport_height * (double(image_width) / image_height);

            w = unit(lookfrom - lookat);   // unit vector going directly backwards from camera
            u = unit(cross(vup, w));       // unit vector going directly right from camera
            v = cross(w, u);               // unit vector going directly up from camera

            // vectors across the horizontal and down the vertical viewport edges
            double3 viewport_u = viewport_width * u;
            double3 viewport_v = viewport_height * -v;

            pixel_du = viewport_u / image_width;
            pixel_dv = viewport_v / image_height;

            // upper left pixel location
            point3 viewport_upper_left = camera_centre - (focus_distance * w) - viewport_u / 2 - viewport_v / 2;
            pixel00_point = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);

            double defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2.0));
            defocus_disc_u = u * defocus_radius;
            defocus_disc_v = v * defocus_radius;
        }

        // constructs a ray from the defocus disc to random point around x,y
        ray get_ray(int x, int y) const {

            vec3 offset = sample_square();
            double3 pixel_sample = pixel00_point
                                 + ((x + offset.x()) * pixel_du)
                                 + ((y + offset.y()) * pixel_dv);
            
            point3 ray_origin = (defocus_angle <= 0) ? camera_centre : defocus_disc_sample();
            vec3 ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        // returns vector to random point in the [-0.5,-0.5]-[0.5,0.5] unit square
        vec3 sample_square() const {
            return point3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disc_sample() const {
            vec3 v = random_in_unit_disc();
            return camera_centre + (v[0] * defocus_disc_u) + (v[1] * defocus_disc_v);
        }

        colour ray_colour(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0) {
                return colour(0.0, 0.0, 0.0);
            }

            hit_record rec;

            // don't go from 0 in order to avoid rounding errors where rays are projecting from slightly away from the object surface
            // fixes shadow acne problem
            // also actually cuts down on processing time a lot, by stopping rays from pissing about and being silly
            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                colour attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_colour(scattered, depth - 1, world);
                }
                return colour(0.0, 0.0, 0.0);
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