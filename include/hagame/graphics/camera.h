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

        float zNear = 0;
        float zFar = -1000.0;

        float zoom = 1.0;
        Vec2 size;
        bool centered;

        Mat4 view() const override {
            return Mat4::Identity();
        }

        Mat4 projection() const override {
            if (centered) {
                return Mat4::Orthographic(
                        transform.position[0] - size[0] * 0.5f * zoom,
                        transform.position[0] + size[0] * 0.5f * zoom,
                        transform.position[1] - size[1] * 0.5f * zoom,
                        transform.position[1] + size[1] * 0.5f * zoom,
                        zFar,
                        zNear
                );
            }
            else {
                return Mat4::Orthographic(
                        0,
                        transform.position[0] + size[0] * zoom,
                        0,
                        transform.position[1] + size[1] * zoom,
                        zFar,
                        zNear
                );
            }
        }

        Vec2 getGamePos(Vec2 screenPos) {
            return (screenPos - size.cast<float>() * 0.5f) * zoom + transform.position.resize<2>();
        }
    };

    class FixedAspectOrthographicCamera : public Camera {
    public:

        float zNear = 0;
        float zFar = -1000.0;

        AspectRatio ratio = AR_16x9;

        float metersPerViewport = 10.0f;

        Vec2 windowSize = Vec2::Zero();

        Mat4 view() const override {
            return Mat4::Identity();
        }

        Rect getViewport() const {
            if (windowSize[0] > windowSize[1] * ratio.ratio()) {
                Vec2 size = Vec2(ratio.ratio() * windowSize[1], windowSize[1]);
                Vec2 pos = Vec2((windowSize[0] - size[0]) * 0.5, 0);
                return Rect(pos, size);
            } else {
                std::cout << "RATIO = " << ratio.inverse() * windowSize[0] << "\n";
                Vec2 size = Vec2(windowSize[0], ratio.inverse() * windowSize[0]);
                std::cout << "SIZE = " << size << "\n";
                Vec2 pos = Vec2(0, (windowSize[1] - size[1]) * 0.5);
                return Rect(pos, size);
            }

        }

        Vec2 getSize() const {
            return Vec2(ratio.ratio(), 1.0) * metersPerViewport;
        }

        Mat4 projection() const override {

            Vec2 size = getSize();

            return Mat4::Orthographic(
                transform.position[0] - size[0] * 0.5f,
                transform.position[0] + size[0] * 0.5f,
                transform.position[1] - size[1] * 0.5f,
                transform.position[1] + size[1] * 0.5f,
                zFar,
                zNear
            );
        }

        Vec2 getGamePos(Vec2 screenPos) {
            Vec2 size = getSize();
            return (screenPos - size.cast<float>() * 0.5f) + transform.position.resize<2>();
        }

    private:



    };
}

#endif //HAGAME2_CAMERA_H
