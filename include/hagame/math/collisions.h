//
// Created by henry on 2/1/23.
//

#ifndef HAGAME2_COLLISIONS_H
#define HAGAME2_COLLISIONS_H

#include <optional>

#include "aliases.h"
#include "ray.h"
#include "triangle.h"

namespace hg::math::collisions {
    struct Hit {
        Vec3 position;
        Vec3 normal;
        float depth;

        // Calculates the vector needed to offset the collision
        Vec3 calcOffset() {
            return position + normal * -depth;
        }
    };

    std::optional<Hit> checkRayAgainstSphere(Ray ray, Sphere sphere, float &t);
    //std::optional<Hit> checkRayAgainstCube(Ray ray, Cube cube, float& t);
    //std::optional<Hit> checkRayAgainstCapsule(Ray ray, Capsule capsule, float& t);
   // std::optional<Hit> checkRayAgainstTriangle(Ray ray, Triangle tri, float& t);
    //std::optional<Hit> checkRayAgainstPlane(Ray ray, Plane plane, float& t);
    //std::optional<Hit> checkRayAgainstDisc(Ray ray, Disc disc, float& t);
    //std::optional<Hit> checkRayAgainstHollowDisc(Ray ray, Vec3 center, Vec3 normal, float innerRadius, float outerRadius, float& t);
}

std::optional<hg::math::collisions::Hit> hg::math::collisions::checkRayAgainstSphere(Ray ray, Sphere sphere, float& t)
{
    std::optional<hg::math::collisions::Hit> hit;

    Vec3 f = ray.origin - sphere.center;
    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(f, ray.direction);
    float c = dot(f, f) - sphere.radius * sphere.radius;

    float disc = b * b - 4 * a * c;

    if (disc < 0) {
        return std::nullopt;
    }
    else {
        disc = sqrt(disc);
        float t1 = (-b - disc) / (2 * a);
        float t2 = (-b + disc) / (2 * a);

        if (t1 >= 0 && t1 <= 1) {
            t = t1;
        }
        else if (t2 >= 0 && t2 <= 1) {
            t = t2;
        }
        else {
            return std::nullopt;
        }

        Vec3 pos = ray.getPointOnLine(t);
        Vec3 norm = (pos - sphere.center).normalized();

        hit = hg::math::collisions::Hit{ pos, norm, (ray.origin + ray.direction - pos).magnitude() };

        return hit;
    }
}

#endif //HAGAME2_COLLISIONS_H
