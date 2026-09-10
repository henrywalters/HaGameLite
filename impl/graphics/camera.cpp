//
// Created by henry on 9/1/24.
//
#include "../../../include/hagame/graphics/camera.h"

hg::Mat4 hg::graphics::OrthographicCamera::projection() const {
    hg::Vec2 visibleSize = size.cast<float>() / pixelsPerMeter * zoom;

    if (centered) {
        return Mat4::Orthographic(
                -visibleSize[0] * 0.5f, visibleSize[0] * 0.5f,
                -visibleSize[1] * 0.5f, visibleSize[1] * 0.5f,
                zNear, zFar
        );
    } else {
        return Mat4::Orthographic(
                0, visibleSize[0],
                0, visibleSize[1],
                zNear, zFar
        );
    }
}

hg::Mat4 hg::graphics::OrthographicCamera::view() const {
    auto position = entity->position();
    if (roundTo > 0) {
        position = position.rounded(roundTo);
    }
    return Mat4::Translation(position.resize<3>() * -1); // camera offset now lives here
}

hg::Vec2 hg::graphics::OrthographicCamera::getGamePos(hg::Vec2 screenPos) {
    auto position = entity->position();
    // pixels from screen center -> world units -> offset by camera world position
    return (screenPos - size.cast<float>() * 0.5f) / pixelsPerMeter * zoom + position.resize<2>();
}

// hg::Mat4 hg::graphics::OrthographicCamera::view() const {
//     return Mat4::Identity();
// }

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

void hg::graphics::OrthographicZoom::update(int scroll, Vec2 mousePos, double dt) {
    auto camera = entity->getComponent<OrthographicCamera>();

    if (scroll != 0.0f && camera) {
        float factor = 1.0f + scroll * zoomSpeed * dt;
        camera->zoom = camera->zoom * factor;
    }
}

void hg::graphics::OrthographicPan::update(bool move, Vec2 mousePos) {

    auto camera = entity->getComponent<OrthographicCamera>();

    if (move && camera) {
        auto delta = mousePos - m_lastMousePos;
        entity->transform.position -= (delta / camera->pixelsPerMeter * camera->zoom).resize<3>();
    }

    m_lastMousePos = mousePos;
}