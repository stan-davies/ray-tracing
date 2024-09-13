#ifndef RAY_H
#define RAY_H

#include "float3.h"

class ray {
    public:
        ray() {}

        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        // returns a constant reference, and doesn't change the internals
        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir; }

        point3 at(float t) const {
            return orig + (t * dir);
        }

    private:
        point3 orig;
        vec3 dir;
};

#endif