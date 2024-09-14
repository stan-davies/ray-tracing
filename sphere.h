#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        // fmax enforces r > 0 by returning the greater value of 0 and radius
        sphere(const point3& centre, double radius, shared_ptr<material> mat) : centre(centre), radius(std::fmax(0, radius)), mat(mat) { }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 origin_to_centre = centre - r.origin();

            double a = r.direction().length_squared();
            double h = dot(r.direction(), origin_to_centre);
            double c = origin_to_centre.length_squared() - (radius * radius);

            double discriminant = h * h - a * c;

            if (discriminant < 0) {
                return false;
            }

            double sqrt_discriminant = std::sqrt(discriminant);

            double root = (h - sqrt_discriminant) / a;
            // prefer first root, if neither is in range then no hit
            if (!ray_t.surrounds(root)) {
                root = (h + sqrt_discriminant) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - centre) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        point3 centre;
        double radius;
        shared_ptr<material> mat;
};

#endif