//
// Created by henry on 3/24/23.
//

#ifndef HAGAME2_CAMERA_H
#define HAGAME2_CAMERA_H

#include "../math/aliases.h"
#include "../math/transform.h"

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
        Vec2i size;
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
}

#endif //HAGAME2_CAMERA_H
