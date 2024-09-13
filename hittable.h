#ifndef HITTABLE_H
#define HITTABLE_H

class hit_record {
    public:
        point3 p;
        vec3 normal;
        float t;
        bool front_face;

        // the normal will ALWAYS point out of the hittable, and we record (in 'front_face') which direction it originally came from 
        // the normal is assumed to be a unit vector
        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, const float ray_tmin, const float ray_tmax, hit_record& rec) const = 0;
};

#endif