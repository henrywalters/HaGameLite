//
// Created by henry on 12/18/22.
//

#include "../../include/hagame/math/transform.h"

void hg::math::Transform::move(hg::Vec3 offset) {
    position += offset;
}

void hg::math::Transform::rotate(hg::Vec3 axis, float rotateBy) {
    rotate(Quat(rotateBy, axis));
}

void hg::math::Transform::rotate(hg::Quat rotateBy) {
    rotation = rotation * rotateBy;
}

void hg::math::Transform::lookAt(Vec3 point) {
    Vec3 forward = normalize(point - position);
    Vec3 face = Vec3::Face();
    float d = dot(forward, face);
    float rot = acos(d);
    Vec3 axis = normalize(cross(face, forward));
    rotation = Quat(rot, axis);
}

hg::Mat4 hg::math::Transform::getModel() {
    return Mat4::Translation(position) * Mat4::Rotation(rotation) * Mat4::Scale(scale);
}

hg::Vec3 hg::math::Transform::top()
{
    return rotation.rotatePoint(hg::Vec3::Top());
}

hg::Vec3 hg::math::Transform::bottom()
{
    return rotation.rotatePoint(hg::Vec3::Bottom());
}

hg::Vec3 hg::math::Transform::right()
{
    return rotation.rotatePoint(hg::Vec3::Right());
}

hg::Vec3 hg::math::Transform::left ()
{
    return rotation.rotatePoint(hg::Vec3::Left());
}

hg::Vec3 hg::math::Transform::face()
{
    return rotation.rotatePoint(hg::Vec3::Face());
}

hg::Vec3 hg::math::Transform::back()
{
    return rotation.rotatePoint(hg::Vec3::Back());
}