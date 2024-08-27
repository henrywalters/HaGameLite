//
// Created by henry on 2/1/23.
//

#ifndef HAGAME2_COLLISIONS_H
#define HAGAME2_COLLISIONS_H

#include <optional>

#include "aliases.h"
#include "ray.h"
#include "triangle.h"

#include "../core/entity.h"
#include "components/rectCollider.h"
#include "components/circleCollider.h"
#include "polygon.h"

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

    hg::Vec3 getCenter(hg::Entity* entity);
    components::Collider* getCollider(hg::Entity* entity);
    std::optional<Hit> checkEntityAgainstEntity(hg::Entity* a, hg::Entity* b);

    std::optional<Hit> checkRayAgainstSphere(Ray ray, Sphere sphere, float &t);
    std::optional<Hit> checkRayAgainstRect(Ray ray, Rect rect, float& t);
    std::optional<Hit> checkRayAgainstCircle(Ray ray, Circle cirle, float& t);
    std::optional<Hit> checkRayAgainstEntity(Ray ray, Entity* entity, float& t);
    std::optional<Hit> checkRayAgainstPolygon(Ray ray, Polygon polygon, float& t);
    std::optional<Hit> checkCircleAgainstLine(Circle circle, LineSegment line);
    std::optional<Hit> checkCircleAgainstCircle(Circle a, Circle b);
    std::optional<Hit> checkCircleAgainstRect(Circle circle, Rect rect);
    //std::optional<Hit> checkRayAgainstCube(Ray ray, Cube cube, float& t);
    //std::optional<Hit> checkRayAgainstCapsule(Ray ray, Capsule capsule, float& t);
   // std::optional<Hit> checkRayAgainstTriangle(Ray ray, Triangle tri, float& t);
    //std::optional<Hit> checkRayAgainstPlane(Ray ray, Plane plane, float& t);
    //std::optional<Hit> checkRayAgainstDisc(Ray ray, Disc disc, float& t);
    //std::optional<Hit> checkRayAgainstHollowDisc(Ray ray, Vec3 center, Vec3 normal, float innerRadius, float outerRadius, float& t);

    std::optional<Hit> checkRectAgainstRect(Rect a, Rect b);
    bool checkRectAgainstPolygon(Rect rect, Polygon polygon);
}

#endif //HAGAME2_COLLISIONS_H
