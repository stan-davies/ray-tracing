#ifndef FLOAT3_H
#define FLOAT3_H

#include <cmath>
#include <iostream>


// i have used float, i just prefer it
// if it gets a bit weird and inprecise, convert it to double

class float3 {
    public:
        float e[3];

        // e is initialised to these values before any code is run
        float3() : e{0.f, 0.f, 0.f} {}
        float3(float e0, float e1, float e2) : e{e0, e1, e2} {}

        // const here indicates that this doesn't change anything internally
        float x() const { return e[0]; }
        float y() const { return e[1]; }
        float z() const { return e[2]; }

        float3 operator-() const { return float3(-e[0], -e[1], -e[2]); }
        float operator[](int i) const { return e[i]; }
        // not got const as it returns a reference to the data, which allows it to be modified
        float& operator[](int i) { return e[i]; }

        float3& operator+=(const float3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        float3& operator*=(float s) {
            e[0] *= s;
            e[1] *= s;
            e[2] *= s;
            return *this;
        }

        float3& operator/=(float s) {
            return *this *= 1/s;
        }
        
        float length() const {
            return std::sqrt(length_squared());
        }

        float length_squared() const {
            return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
        }
};

// just an alias, useful for readability
using point3 = float3;

// inline means that these functions are a part of the above class
// the reason they are not defined inside of it is because are used 
// on instances of the class, returning new instances, rather than 
// being invoked by an instance, and altering its internal data

inline std::ostream& operator<<(std::ostream& out, const float3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline float3 operator+(const float3& u, const float3& v) {
    return float3(
        u.e[0] + v.e[0], 
        u.e[1] + v.e[1],
        u.e[2] + v.e[2]
    );
}

inline float3 operator-(const float3& u, const float3& v) {
    return float3(
        u.e[0] - v.e[0],
        u.e[1] - v.e[1],
        u.e[2] - v.e[2]
    );
}

inline float3 operator*(const float3& u, const float3& v) {
    return float3(
        u.e[0] * v.e[0],
        u.e[1] * v.e[1],
        u.e[2] * v.e[2]
    );
}

inline float3 operator*(const float s, const float3& v) {
    return float3(
        s * v.e[0],
        s * v.e[1],
        s * v.e[2]
    );
}

inline float3 operator*(const float3& v, const float s) {
    return s * v;
}

inline float3 operator/(const float3& v, const float s) {
    return (1/s) * v;
}

inline float dot(const float3& u, const float3& v) {
    return (u.e[0] * v.e[0])
         + (u.e[1] * v.e[1])
         + (u.e[2] * v.e[2]);
}

inline float3 cross(const float3& u, const float3& v) {
    return float3(
        (u.e[1] * v.e[2]) - (u.e[2] * v.e[1]),
        (u.e[2] * v.e[0]) - (u.e[0] * v.e[2]),
        (u.e[0] * v.e[1]) - (u.e[1] * v.e[0])
    );
}

inline float3 unit(const float3& v) {
    return v / v.length();
}

#endif