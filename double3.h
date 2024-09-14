#ifndef DOUBLE3_H
#define DOUBLE3_H

// i have used double, i just prefer it
// if it gets a bit weird and inprecise, convert it to double

class double3 {
    public:
        double e[3];

        // e is initialised to these values before any code is run
        double3() : e{0.0, 0.0, 0.0} {}
        double3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        // const here indicates that this doesn't change anything internally
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        double3 operator-() const { return double3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        // not got const as it returns a reference to the data, which allows it to be modified
        double& operator[](int i) { return e[i]; }

        double3& operator+=(const double3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        double3& operator*=(double s) {
            e[0] *= s;
            e[1] *= s;
            e[2] *= s;
            return *this;
        }

        double3& operator/=(double s) {
            return *this *= 1/s;
        }
        
        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
        }

        bool near_zero() const {
            double small = 1e-8;
            return (std::fabs(e[0]) < small) && (std::fabs(e[1]) < small) && (std::fabs(e[2]) < small);
        }

        static double3 random() {
            return double3(
                random_double(), 
                random_double(), 
                random_double()
            );
        }

        static double3 random(double min, double max) {
            return double3(
                random_double(min, max), 
                random_double(min, max),
                random_double(min, max)
            );
        }
};

// just an alias, useful for readability
using point3 = double3;
using vec3 = double3;

// inline means that these functions are a part of the above class
// the reason they are not defined inside of it is because are used 
// on instances of the class, returning new instances, rather than 
// being invoked by an instance, and altering its internal data

inline std::ostream& operator<<(std::ostream& out, const double3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline double3 operator+(const double3& u, const double3& v) {
    return double3(
        u.e[0] + v.e[0], 
        u.e[1] + v.e[1],
        u.e[2] + v.e[2]
    );
}

inline double3 operator-(const double3& u, const double3& v) {
    return double3(
        u.e[0] - v.e[0],
        u.e[1] - v.e[1],
        u.e[2] - v.e[2]
    );
}

inline double3 operator*(const double3& u, const double3& v) {
    return double3(
        u.e[0] * v.e[0],
        u.e[1] * v.e[1],
        u.e[2] * v.e[2]
    );
}

inline double3 operator*(const double s, const double3& v) {
    return double3(
        s * v.e[0],
        s * v.e[1],
        s * v.e[2]
    );
}

inline double3 operator*(const double3& v, const double s) {
    return s * v;
}

inline double3 operator/(const double3& v, const double s) {
    return (1/s) * v;
}

inline double dot(const double3& u, const double3& v) {
    return (u.e[0] * v.e[0])
         + (u.e[1] * v.e[1])
         + (u.e[2] * v.e[2]);
}

inline double3 cross(const double3& u, const double3& v) {
    return double3(
        (u.e[1] * v.e[2]) - (u.e[2] * v.e[1]),
        (u.e[2] * v.e[0]) - (u.e[0] * v.e[2]),
        (u.e[0] * v.e[1]) - (u.e[1] * v.e[0])
    );
}

inline double3 unit(const double3& v) {
    return v / v.length();
}

// generate random point inside unit disc
inline vec3 random_in_unit_disc() {
    while (true) {
        vec3 v = vec3(random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0);
        if (v.length_squared() < 1.0) {
            return v;
        }
    }
}

inline vec3 random_unit() {
    while (true) {
        vec3 v = vec3::random(-1.0, 1.0);
        double len_sq = v.length_squared();

        // eliminate anything outside unit sphere
        // also eliminate vectors that are very short, as rounding errors would likely lead to a divide by 0 with such vectors
        if (1e-160 < len_sq && len_sq <= 1) {
            return v / sqrt(len_sq);
        }
    }
}

// random unit vector that projects outwards from a hemisphere
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit();

    if (dot(on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

// reflects a vector on the normal at a point
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

// it refracts i promise lol
// uv is the refracted ray
// n is the normal
// etai_over_etat is the quotient of the angles between the incoming ray and the normal, and the outgoing (or rather ingoing) ray and the normal
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    double cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perpendicular = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perpendicular.length_squared())) * n;
    return r_out_perpendicular + r_out_parallel;
}

#endif