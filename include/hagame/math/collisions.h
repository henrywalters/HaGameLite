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
    std::optional<Hit> checkRayAgainstRect(Ray ray, Rect rect, float& t);
    //std::optional<Hit> checkRayAgainstCube(Ray ray, Cube cube, float& t);
    //std::optional<Hit> checkRayAgainstCapsule(Ray ray, Capsule capsule, float& t);
   // std::optional<Hit> checkRayAgainstTriangle(Ray ray, Triangle tri, float& t);
    //std::optional<Hit> checkRayAgainstPlane(Ray ray, Plane plane, float& t);
    //std::optional<Hit> checkRayAgainstDisc(Ray ray, Disc disc, float& t);
    //std::optional<Hit> checkRayAgainstHollowDisc(Ray ray, Vec3 center, Vec3 normal, float innerRadius, float outerRadius, float& t);

    bool checkRectAgainstRect(Rect a, Rect b);
}

#endif //HAGAME2_COLLISIONS_H
