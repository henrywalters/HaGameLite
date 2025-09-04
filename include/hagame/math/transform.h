//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_TRANSFORM_H
#define HAGAME2_TRANSFORM_H

#include "aliases.h"
#include "../utils/watcher.h"

namespace hg::math {
    struct Transform {

        Vec3 position = Vec3::Zero();
        Vec3 positionOffset = Vec3::Zero();
        Vec3 scale = Vec3::Identity();
        Quat rotation = Quat(0, Vec3::Face());

        void move(Vec3 offset);
        void rotate(Vec3 axis, float rotateBy);
        void rotate(Quat rotateBy);
        void lookAt(Vec3 point);

        // Mat4 getModel();

        Vec3 face();
        Vec3 back();
        Vec3 right();
        Vec3 left();
        Vec3 top();
        Vec3 bottom();
    };
}

#endif //HAGAME2_TRANSFORM_H
