//
// Created by henry on 3/24/23.
//

#ifndef HAGAME2_CAMERA_H
#define HAGAME2_CAMERA_H

#include "../math/aliases.h"
#include "../math/transform.h"
#include "aspectRatios.h"

namespace hg::graphics {
    class Camera {
    public:

        math::Transform transform;

        virtual Mat4 view() const {
            return Mat4::Identity();
        };

        virtual Mat4 projection() const {
            return Mat4::Identity();
        }
    };

    class OrthographicCamera : public Camera {
    public:

        float zNear = -1000;
        float zFar = 1000.0;

        float zoom = 1.0;
        Vec2 size;
        bool centered;

        Mat4 projection() const override;

        Vec2 getGamePos(Vec2 screenPos);
    };

    class FixedAspectOrthographicCamera : public Camera {
    public:

        float zNear = 0;
        float zFar = -1000.0;

        AspectRatio ratio = AR_16x9;

        float metersPerViewport = 10.0f;

        Vec2 windowSize = Vec2::Zero();

        Rect getViewport() const;

        Vec2 getSize() const;

        Mat4 projection() const override;

        Vec2 getGamePos(Vec2 screenPos);
    };

    class PerspectiveCamera : public Camera {
    public:

        hg::Vec3 up = hg::Vec3::Top();
        hg::Vec3 forward = hg::Vec3::Face();

        float fov = hg::math::PI / 3.0;
        float aspect = 1.0f;
        float zNear = 0.001f;
        float zFar = 1000.0f;

        virtual Mat4 view() const override;

        virtual Mat4 projection() const override;
    };

    class ObliqueCamera : public Camera {
    public:

        float theta = M_PI / 4;
        float phi = M_PI / 4;
        float zoom = 1.0;
        Vec2 size;
        float zNear = -100.0f;
        float zFar = 100.0f;

        virtual Mat4 view() const override;
        virtual Mat4 projection() const override;
    };
}

#endif //HAGAME2_CAMERA_H
