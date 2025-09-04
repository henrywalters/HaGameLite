//
// Created by henry on 9/1/24.
//
#include "../../../include/hagame/graphics/camera.h"

hg::Mat4 hg::graphics::OrthographicCamera::projection() const {

    auto position = entity->position();

    if (roundTo > 0) {
        position = position.rounded(roundTo);
    }

    if (centered) {
        return Mat4::Orthographic(
                position[0] - size[0] * 0.5f * zoom,
                position[0] + size[0] * 0.5f * zoom,
                position[1] - size[1] * 0.5f * zoom,
                position[1] + size[1] * 0.5f * zoom,
                zNear,
                zFar
        );
    }
    else {
        return Mat4::Orthographic(
                position[0],
                position[0] + size[0] * zoom,
                position[1],
                position[1] + size[1] * zoom,
                zNear,
                zFar
        );
    }
}

hg::Vec2 hg::graphics::OrthographicCamera::getGamePos(hg::Vec2 screenPos) {
    auto position = entity->position();
    return (screenPos - size.cast<float>() * 0.5f) * zoom + position.resize<2>();
}

hg::Mat4 hg::graphics::OrthographicCamera::view() const {
    return Mat4::Identity();
}

hg::Rect hg::graphics::FixedAspectOrthographicCamera::getViewport() const {
    if (windowSize[0] > windowSize[1] * ratio.ratio()) {
        Vec2 size = Vec2(ratio.ratio() * windowSize[1], windowSize[1]);
        Vec2 pos = Vec2((windowSize[0] - size[0]) * 0.5, 0);
        return Rect(pos, size);
    } else {
        Vec2 size = Vec2(windowSize[0], ratio.inverse() * windowSize[0]);
        Vec2 pos = Vec2(0, (windowSize[1] - size[1]) * 0.5);
        return Rect(pos, size);
    }
}

hg::Vec2 hg::graphics::FixedAspectOrthographicCamera::getSize() const {
    return Vec2(ratio.ratio(), 1.0) * metersPerViewport;
}

hg::Mat4 hg::graphics::FixedAspectOrthographicCamera::projection() const {
    Vec2 size = getSize();

    auto position = entity->position();

    return Mat4::Orthographic(
            position[0] - size[0] * 0.5f,
            position[0] + size[0] * 0.5f,
            position[1] - size[1] * 0.5f,
            position[1] + size[1] * 0.5f,
            zFar,
            zNear
    );
}

hg::Vec2 hg::graphics::FixedAspectOrthographicCamera::getGamePos(hg::Vec2 screenPos) {
    Vec2 size = getSize();
    auto position = entity->position();
    return (screenPos - size.cast<float>() * 0.5f) + position.resize<2>();
}

hg::Mat4 hg::graphics::PerspectiveCamera::view() const {
    auto position = entity->position();
    return Mat4::LookAt(
        position,
        entity->rotation().rotatePoint(forward) + position,
        up
    );
}

hg::Mat4 hg::graphics::PerspectiveCamera::projection() const {
    return Mat4::Perspective(fov * math::DEG2RAD, aspect, zNear, zFar);
}

hg::Mat4 hg::graphics::ObliqueCamera::view() const {
    auto position = entity->position();
    // return Mat4::Identity();
    return Mat4::LookAt(
            position,
            entity->rotation().rotatePoint(forward) + position,
            up
    );
}

hg::Mat4 hg::graphics::ObliqueCamera::projection() const {
    auto position = entity->position();
    auto orth = Mat4::Orthographic(
            position[0],
            position[0] + size[0] * zoom,
            position[1],
            position[1] + size[1] * zoom,
            zNear,
            zFar
    );

    Mat4 mat = Mat4::Identity();
    mat.set(0, 2, -(std::cos(theta) / std::sin(theta)));
    mat.set(1, 2, -(std::cos(phi) / std::sin(phi)));
    // mat.set(2, 2, 0);
    return mat * orth;

}
