#include "../../include/hagame/math/collisions.h"
#include "../../include/hagame/math/lineIntersection.h"
#include "../../include/hagame/math/components/circleCollider.h"
#include "../../include/hagame/math/components/rectCollider.h"


using namespace hg;
using namespace hg::math;
using namespace hg::math::collisions;

std::optional<Hit> collisions::checkEntityAgainstEntity(hg::Entity* a, hg::Entity* b) {
    if (a->hasComponent<components::RectCollider>() && b->hasComponent<components::RectCollider>()) {
        return checkRectAgainstRect(a->getComponent<components::RectCollider>()->getRect(), b->getComponent<components::RectCollider>()->getRect());
    } else if (a->hasComponent<components::RectCollider>() && b->hasComponent<components::CircleCollider>()) {
        return checkCircleAgainstRect(b->getComponent<components::CircleCollider>()->getCircle(), a->getComponent<components::RectCollider>()->getRect());
    } else if (b->hasComponent<components::RectCollider>() && a->hasComponent<components::CircleCollider>()) {
        return checkCircleAgainstRect(a->getComponent<components::CircleCollider>()->getCircle(), b->getComponent<components::RectCollider>()->getRect());
    } else if (a->hasComponent<components::CircleCollider>() && b->hasComponent<components::CircleCollider>()) {
        return checkCircleAgainstCircle(a->getComponent<components::CircleCollider>()->getCircle(), b->getComponent<components::CircleCollider>()->getCircle());
    } else {
        return std::nullopt;
    }
}


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


std::optional<Hit> hg::math::collisions::checkRectAgainstRect(Rect a, Rect b) {
    if (!a.intersects(b)) {
        return std::nullopt;
    }

    auto closestPointOnB = b.closestPoint(a.getCenter());
    auto closestPointOnA = a.closestPoint(b.getCenter());
    auto delta = closestPointOnA - closestPointOnB;

    return Hit{
        closestPointOnB.resize<3>(),
        delta.normalized().resize<3>(),
        delta.magnitude()
    };
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

    Vec2 a = ray.origin.resize<2>();
    Vec2 b = (ray.origin + ray.direction).resize<2>();

    LineIntersection intersection;

    for (const auto& edge : polygon) {
        if (linesIntersect(a, b, edge.a, edge.b, intersection)) {
            if (!hasHit || intersection.t() < t) {
                hasHit = true;
                t = intersection.t();
            }
        }
    }

    if (!hasHit) {
        return std::nullopt;
    }

    Hit hit;
    hit.position = ray.getPointOnLine(t);
    hit.normal = ray.direction.normalized() * -1;
    hit.depth = (ray.getPointOnLine(1.0) - hit.position).magnitude();

    return hit;
}

std::optional<Hit> collisions::checkCircleAgainstLine(Circle circle, LineSegment line) {
    Vec2 a = line.a.resize<2>();
    Vec2 b = line.b.resize<2>();

    auto hit = [circle](Vec2 point) {
        auto delta = (point - circle.center).resize<3>();
        Hit out;
        out.normal = delta.normalized();
        out.position = point.resize<3>();
        out.depth = circle.radius - delta.magnitude();
        return out;
    };

    if (circle.contains(a)) {
        return hit(a);
    } else if (circle.contains(b)) {
        return hit(b);
    }

    Vec2 delta = b - a;

    float dot = delta.dot(circle.center - a) / delta.magnitudeSq();
    Vec2 closestPoint = a + delta * dot;

    if (!line.contains(closestPoint.resize<3>())) {
        return std::nullopt;
    }

    if ((closestPoint - circle.center).magnitude() <= circle.radius) {
        return hit(closestPoint);
    }

    return std::nullopt;
}

std::optional<Hit> collisions::checkCircleAgainstCircle(Circle a, Circle b) {
    auto delta = b.center - a.center;
    float mag = delta.magnitude();
    if (mag > a.radius + b.radius) {
        return std::nullopt;
    }

    float t = a.radius / mag;

    return Hit { (a.center + delta * t).resize<3>(), (delta.normalized()).resize<3>(), mag - a.radius - b.radius };
}

std::optional<Hit> collisions::checkCircleAgainstRect(Circle circle, Rect rect) {
    Vec3 pos = rect.pos.resize<3>();
    Vec3 size = rect.size.resize<3>();
    Vec3 cPos = circle.center.resize<3>();
    std::array<LineSegment, 4> lines = {
        LineSegment(pos + Vec3(0, 0, 0), pos + Vec3(size[0], 0, 0)),
        LineSegment(pos + Vec3(size[0], 0, 0), pos + Vec3(size[0], size[1], 0)),
        LineSegment(pos + Vec3(size[0], size[1], 0), pos + Vec3(0, size[1], 0)),
        LineSegment(pos + Vec3(0, size[1], 0), pos + Vec3(0, 0, 0))
    };

    Vec3 closestPoint;
    float minDistance;

    for (int i = 0; i < 4; i++) {
        float t;
        auto point = lines[i].closestPoint(cPos, t);
        float dist = distance(point, cPos);
        if (i == 0 || dist < minDistance) {
            minDistance = dist;
            closestPoint = point;
        }
    }

    if (!rect.contains(circle.center) && minDistance > circle.radius) {
        return std::nullopt;
    }

    auto delta = (closestPoint - cPos).resize<3>();
    Hit out;
    out.normal = delta.normalized();
    out.position = closestPoint;
    out.depth = circle.radius - delta.magnitude();
    return out;
}

