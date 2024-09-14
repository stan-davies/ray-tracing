#include "ray_tracing.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <chrono>

int main() {
    // # world
    hittable_list world;

    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));

    camera cam;

    cam.aspect_ratio       = 16.0 / 9.0;
    cam.image_width        = 400;
    cam.samples_per_pixel  = 100;

    // for timing
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    cam.render(world);

    // conclusion of timer
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
    std::clog << "time in seconds: " << duration.count() << std::flush;
}