#include <iostream>

int main() {
    const int image_width = 256;
    const int image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            float r = float(x) / (image_width - 1);
            float g = float(y) / (image_height - 1);
            float b = 0.f;

            int ir = int(255.999f * r);
            int ig = int(255.999f * g);
            int ib = int(255.999f * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}