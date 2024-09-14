#include "ray_tracing.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <chrono>

int main() {
    // # world
    hittable_list world;

    // the colours are each of the albedo's, so that is essentially the colour that they will end up
    std::shared_ptr<lambertian> material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    std::shared_ptr<lambertian> material_centre = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
    std::shared_ptr<dielectric> material_left   = make_shared<dielectric>(1.5);
    std::shared_ptr<dielectric> material_bubble = make_shared<dielectric>(1.00 / 1.50);
    std::shared_ptr<metal>      material_right  = make_shared<metal>(colour(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_centre));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio       = 16.0 / 9.0;
    cam.image_width        = 400;
    cam.samples_per_pixel  = 100;
    cam.max_bounces        = 50;

    // for timing
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    cam.render(world);

    // conclusion of timer
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
    std::clog << "time in seconds: " << duration.count() << std::flush;
}