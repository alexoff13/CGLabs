#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include "stdc++.h"

#define MAX_RAY_DEPTH 5

template<typename T>
class Vec3 {
public:
    T x, y, z;

    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}

    Vec3(T xx) : x(xx), y(xx), z(xx) {}

    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

    Vec3 &normalize() {
        T nor2 = length2();
        if (nor2 > 0) {
            T invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
    }

    Vec3<T> operator*(const T &f) const { return Vec3<T>(x * f, y * f, z * f); }

    Vec3<T> operator*(const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }

    T dot(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }

    Vec3<T> operator-(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }

    Vec3<T> operator+(const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }

    Vec3<T> &operator+=(const Vec3<T> &v) {
        x += v.x, y += v.y, z += v.z;
        return *this;
    }

    Vec3<T> &operator*=(const Vec3<T> &v) {
        x *= v.x, y *= v.y, z *= v.z;
        return *this;
    }

    Vec3<T> operator-() const { return Vec3<T>(-x, -y, -z); }

    T length2() const { return x * x + y * y + z * z; }

    friend std::ostream &operator<<(std::ostream &os, const Vec3<T> &v) {
        os << "[" << v.x << " " << v.y << " " << v.z << "]";
        return os;
    }
};

typedef Vec3<float> Vec3f;

class Sphere {
public:
    Vec3f center;                           /// position of the sphere
    float radius, radius2;                  /// sphere radius and radius^2
    Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
    float transparency, reflection;         /// surface transparency and reflectivity
    Sphere(
            const Vec3f &c,
            const float &r,
            const Vec3f &sc,
            const float &refl = 0,
            const float &transp = 0,
            const Vec3f &ec = 0) :
            center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec),
            transparency(transp), reflection(refl) { /* empty */ }

    bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const {
        Vec3f l = center - rayorig;
        float tca = l.dot(raydir);
        if (tca < 0) return false;
        float d2 = l.dot(l) - tca * tca;
        if (d2 > radius2) return false;
        float thc = std::sqrt(radius2 - d2);
        t0 = tca - thc;
        t1 = tca + thc;

        return true;
    }
};

float mix(const float &a, const float &b, const float &mix) {
    return b * mix + a * (1 - mix);
}

Vec3f trace(
        const Vec3f &rayorig,
        const Vec3f &raydir,
        const std::vector<Sphere> &spheres,
        const int &depth) {
    //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
    float tnear = INFINITY;
    const Sphere *sphere = NULL;
    // find intersection of this ray with the sphere in the scene
    for (unsigned i = 0; i < spheres.size(); ++i) {
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < tnear) {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }

    if (!sphere) return Vec3f(2);
    Vec3f surfaceColor = 0;
    Vec3f phit = rayorig + raydir * tnear;
    Vec3f nhit = phit - sphere->center;
    nhit.normalize();

    float bias = 1e-4;
    bool inside = false;
    if (raydir.dot(nhit) > 0) nhit = -nhit, inside = true;
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) {
        float facingratio = -raydir.dot(nhit);
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);

        Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit);
        refldir.normalize();
        Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1);
        Vec3f refraction = 0;

        if (sphere->transparency) {
            float ior = 1.1, eta = (inside) ? ior : 1 / ior;
            float cosi = -nhit.dot(raydir);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec3f refrdir = raydir * eta + nhit * (eta * cosi - sqrt(k));
            refrdir.normalize();
            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
        }

        surfaceColor = (
                               reflection * fresneleffect +
                               refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
    } else {
        for (unsigned i = 0; i < spheres.size(); ++i) {
            if (spheres[i].emissionColor.x > 0) {
                Vec3f transmission = 1;
                Vec3f lightDirection = spheres[i].center - phit;
                lightDirection.normalize();
                for (unsigned j = 0; j < spheres.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
                            transmission = 0;
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission *
                                std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
            }
        }
    }

    return surfaceColor + sphere->emissionColor;
}

void render(const std::vector<Sphere> &spheres) {
    unsigned width = 1920, height = 1080;
    Vec3f *image = new Vec3f[width * height], *pixel = image;
    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = 30, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);
    // Trace rays
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x, ++pixel) {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = trace(Vec3f(0), raydir, spheres, 0);
        }
    }

    std::ofstream ofs("../result_retracing.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char) (std::min(float(1), image[i].x) * 255) <<
            (unsigned char) (std::min(float(1), image[i].y) * 255) <<
            (unsigned char) (std::min(float(1), image[i].z) * 255);
    }
    ofs.close();
    delete[] image;
}


int main() {
    srand(13);
    std::vector<Sphere> spheres;
    spheres.emplace_back(Vec3f(0.0, 0, -20), 4, Vec3f(0.00, 0.5, 0.1), 1, 0.5);
    spheres.emplace_back(Vec3f(5.0, -1, -15), 2, Vec3f(0.10, 0.16, 0.66), 1, 0.0);
    spheres.emplace_back(Vec3f(0.0, 20, -30), 3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3));
    render(spheres);

    return 0;
}