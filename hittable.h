#ifndef HITTABLE_H
#define HITTABLE_H

// solves a circular reference issue, as material gets included way elsewhere so we need this to tell the compiler that it's all going to be alright
class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
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

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif