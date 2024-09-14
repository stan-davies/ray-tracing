#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

// this shouldn't need to be here but vscode SUCKS
#ifndef RAY_TRACING_H
#include "ray_tracing.h"
#endif

class material {
    public: 
        virtual ~material() = default;

        virtual bool scatter( const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) 
        const {
            return false;
        }
};

class lambertian : public material {
    public:
        lambertian(const colour& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered)
        const override {
            vec3 scatter_direction = rec.normal + random_unit();

            // catch very small scatter directions to avoid later processing issues
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        colour albedo;
};

class metal : public material {
    public:
        metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

        bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered)
        const override {
            // because the reflection is sharp rather than random and diffuse, and all the rays therefore follow set paths, this will appear reflective (in the sense of like a mirro)
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            // a fuzz-scaled unit vector at the end of the reflection vector, we then add these two, slightly randomising the direction and adding fuzziness
            reflected = unit(reflected) + (fuzz * random_unit());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        colour albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered)
        const override {
            // this uses an implementation of Snell's law
            attenuation = colour(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = unit(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = (ri * sin_theta > 1.0);
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        // how wild the angle go when light refracts in teh material
        // ratio of the RI of the outside medium to the RI of the inside medium
        double refraction_index;

        // uses Schlick's approximation
        static double reflectance(double cosine, double refraction_index) {
            double r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }
};

#endif