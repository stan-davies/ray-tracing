#include "ray_tracing.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include <chrono>

int main() {
    // # world
    hittable_list world;

    shared_ptr<lambertian> ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, ground_material));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            double choose_mat = random_double();
            point3 centre(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((centre - point3(4.0, 0.2, 0.0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    colour albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    // metal
                    colour albedo = colour::random(0.5, 1.0);
                    double fuzz = random_double(0.0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                }
                
                world.add(make_shared<sphere>(centre, 0.2, sphere_material));
            }
        }
    }

    shared_ptr<dielectric> glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0.0, 1.0, 0.0), 1.0, glass));

    shared_ptr<lambertian> diffuse = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4.0, 1.0, 0.0), 1.0, diffuse));

    shared_ptr<metal> silver = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4.0, 1.0, 0.0), 1.0, silver));

    camera cam;

    cam.aspect_ratio       = 16.0 / 9.0;
    cam.image_width        = 1200;
    cam.samples_per_pixel  = 500;
    cam.max_bounces        = 50;

    cam.vfov      = 20;
    cam.lookfrom  = point3(13.0, 2.0, 3.0);
    cam.lookat    = point3( 0.0, 0.0, 0.0);
    cam.vup       = vec3(0.0, 1.0, 0.0);

    cam.defocus_angle  = 0.6;
    cam.focus_distance = 10.0;

    // for timing
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    cam.render(world);

    // conclusion of timer
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
    std::clog << "time in seconds: " << duration.count() << std::flush;
}