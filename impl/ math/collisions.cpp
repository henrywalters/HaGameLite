#include "../../include/hagame/math/collisions.h"
#include "../../include/hagame/math/lineIntersection.h"

using namespace hg;
using namespace hg::math;
using namespace hg::math::collisions;

std::optional<Hit> hg::math::collisions::checkRayAgainstSphere(Ray ray, Sphere sphere, float& t)
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

std::optional<Hit> hg::math::collisions::checkRayAgainstRect(Ray ray, Rect rect, float& t)
{
    auto tNear = (rect.pos - ray.origin.resize<2>()).div(ray.direction.resize<2>());
    auto tFar = (rect.pos + rect.size - ray.origin.resize<2>()).div(ray.direction.resize<2>());

    if (std::isnan(tFar[1]) || std::isnan(tFar[0])) return std::nullopt;
    if (std::isnan(tNear[0]) || std::isnan(tNear[1])) return std::nullopt;

    if (tNear[0] > tFar[0]) std::swap(tNear[0], tFar[0]);
    if (tNear[1] > tFar[1]) std::swap(tNear[1], tFar[1]);

    if (tNear[0] > tFar[1] || tNear[1] > tFar[0]) return std::nullopt;

    auto tHitNear = std::max(tNear[0], tNear[1]);
    auto tHitFar = std::min(tFar[0], tFar[1]);

    if (tHitFar < 0 || tHitNear < 0) return std::nullopt;

    hg::math::collisions::Hit hit;

    hit.position = ray.getPointOnLine(tHitNear);
    hit.depth = ray.direction.magnitude() * (1 - tHitNear);

    if (tNear[0] > tNear[1]) {
        if (ray.direction[0] < 0)
            hit.normal = Vec3(1, 0, 0);
        else
            hit.normal = Vec3(-1, 0, 0);
    }
    else if (tNear[0] < tNear[1]) {
        if (ray.direction[1] < 0) {
            hit.normal = Vec3(0, 1, 0);
        }
        else {
            hit.normal = Vec3(0, -1, 0);
        }
    }

    t = tHitNear;

    return hit;
}

std::optional<Hit> hg::math::collisions::checkRayAgainstCircle(Ray ray, Circle circle, float& t) {
    Vec2 offset = ray.origin.resize<2>() - circle.center;

    float a = ray.direction.x() * ray.direction.x() + ray.direction.y() * ray.direction.y();
    float b = 2 * (offset.x() * ray.direction.x() + offset.y() * ray.direction.y());
    float c = offset.x() * offset.x() + offset.y() * offset.y() - circle.radius * circle.radius;

    float disc = b * b - 4 * a * c;

    if (disc < 0) {
        return std::nullopt;
    }

    float t1 = (-b + std::sqrt(disc)) / (2 * a);
    float t2 = (-b - std::sqrt(disc)) / (2 * a);

    Hit hit;
    hit.normal = ray.direction.normalized() * -1;
    hit.depth = 0;

    if (t1 > t2) {
        t = t2;
        hit.position = ray.getPointOnLine(t2);
    } else {
        t = t1;
        hit.position = ray.getPointOnLine(t1);
    }

    return hit;
}


bool hg::math::collisions::checkRectAgainstRect(Rect a, Rect b) {
    return a.pos.x() <= b.pos.x() + b.size.x() && a.pos.x() + a.size.x() >= b.pos.x() &&
        a.pos.y() <= b.pos.y() + b.size.y() && a.pos.y() + a.size.y() >= b.pos.y();
}

std::optional<Hit> hg::math::collisions::checkRayAgainstEntity(Ray ray, Entity *entity, float &t) {

    if (!entity) {
        return std::nullopt;
    }

    auto rect = entity->getComponent<components::RectCollider>();

    if (rect) {
        rect->getRect().pos = entity->position().resize<2>();
        return hg::math::collisions::checkRayAgainstRect(ray, rect->getRect(), t);
    }

    auto circle = entity->getComponent<components::CircleCollider>();
    if (circle) {
        circle->getCircle().center = entity->position().resize<2>();
        return hg::math::collisions::checkRayAgainstCircle(ray, circle->getCircle(), t);
    }

    return std::nullopt;
}

bool hg::math::collisions::checkRectAgainstPolygon(Rect rect, Polygon polygon) {
    std::array<Vec2, 4> points = {
        rect.pos,
        rect.pos + Vec2(rect.size.x(), 0),
        rect.pos + rect.size,
        rect.pos + Vec2(0, rect.size.y()),
    };

    LineIntersection intersection;

    for (const auto& edge : polygon) {
        for (int i = 0; i < 4; i++) {
            if (linesIntersect(points[i], points[(i + 1) % 4], edge.a, edge.b, intersection)) {
                return true;
            }
        }
    }

    return false;
}

std::optional<Hit> collisions::checkRayAgainstPolygon(Ray ray, Polygon polygon, float &t) {
    bool hasHit = false;
    float minT;

    Vec2 a = ray.origin.resize<2>();
    Vec2 b = (ray.origin + ray.direction).resize<2>();

    LineIntersection intersection;

    for (const auto& edge : polygon) {
        if (linesIntersect(a, b, edge.a, edge.b, intersection)) {
            if (!hasHit || intersection.t() < minT) {
                hasHit = true;
                minT = intersection.t();
            }
        }
    }

    if (!hasHit) {
        return std::nullopt;
    }

    Hit hit;
    hit.position = ray.getPointOnLine(minT);
    hit.normal = ray.direction.normalized() * -1;
    hit.depth = (ray.getPointOnLine(1.0) - hit.position).magnitude();

    return hit;
}

