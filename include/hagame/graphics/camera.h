//
// Created by henry on 3/24/23.
//

#ifndef HAGAME2_CAMERA_H
#define HAGAME2_CAMERA_H

#include "../core/component.h"
#include "../core/entity.h"
#include "../math/aliases.h"
#include "../math/transform.h"
#include "aspectRatios.h"

namespace hg::graphics {
    class Camera : public hg::Component {
    public:

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

        float roundTo = 0;

        Mat4 projection() const override;
        Mat4 view() const override;

        Vec2 getGamePos(Vec2 screenPos);

    protected:

        OBJECT_NAME(OrthographicCamera)

    };

    HG_COMPONENT(Graphics, OrthographicCamera)
    HG_NUMBER_FIELD(OrthographicCamera, float, zNear, -10000, 10000, 1)
    HG_NUMBER_FIELD(OrthographicCamera, float, zFar, -10000, 10000, 1)
    HG_NUMBER_FIELD(OrthographicCamera, float, zoom, 0.0000001, 10, 0.001)
    HG_FIELD(OrthographicCamera, hg::Vec2, size)
    HG_FIELD(OrthographicCamera, bool, centered)
    HG_NUMBER_FIELD(OrthographicCamera, float, roundTo, 0, 100, 0.01);


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

        float fov = 75;
        float aspect = 1.0f;
        float zNear = 0.001f;
        float zFar = 1000.0f;

        virtual Mat4 view() const override;

        virtual Mat4 projection() const override;

    protected:

        OBJECT_NAME(PerspectiveCamera)

    };

    HG_COMPONENT(Graphics, PerspectiveCamera)
    HG_FIELD(PerspectiveCamera, hg::Vec3, up)
    HG_FIELD(PerspectiveCamera, hg::Vec3, forward)
    HG_NUMBER_FIELD(PerspectiveCamera, float, fov, 1, 180, 1)
    HG_NUMBER_FIELD(PerspectiveCamera, float, aspect, 0, 1, 0.01)
    HG_NUMBER_FIELD(PerspectiveCamera, float, zNear, -10000, 10000, 1)
    HG_NUMBER_FIELD(PerspectiveCamera, float, zFar, -10000, 10000, 1)

    class ObliqueCamera : public Camera {
    public:

        hg::Vec3 up = hg::Vec3::Top();
        hg::Vec3 forward = hg::Vec3::Face();

        float theta = M_PI / 4;
        float phi = M_PI / 4;
        float zoom = 1.0;
        Vec2 size;
        float zNear = 0;
        float zFar = 100.0f;

        virtual Mat4 view() const override;
        virtual Mat4 projection() const override;
    };

    HG_COMPONENT(Graphics, ObliqueCamera)
    HG_FIELD(ObliqueCamera, hg::Vec3, up)
    HG_FIELD(ObliqueCamera, hg::Vec3, forward)
    HG_FIELD(ObliqueCamera, hg::Vec2, size)
    HG_NUMBER_FIELD(ObliqueCamera, float, theta, 0, 180, 1)
    HG_NUMBER_FIELD(ObliqueCamera, float, phi, 0, 180, 1)
    HG_NUMBER_FIELD(ObliqueCamera, float, zNear, -10000, 10000, 1)
    HG_NUMBER_FIELD(ObliqueCamera, float, zFar, -10000, 10000, 1)
    HG_NUMBER_FIELD(ObliqueCamera, float, zoom, 0.0000001, 10, 0.001)

}

#endif //HAGAME2_CAMERA_H
