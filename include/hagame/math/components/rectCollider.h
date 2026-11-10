//
// Created by henry on 10/15/23.
//

#ifndef HAGAME2_RECTCOLLIDER_H
#define HAGAME2_RECTCOLLIDER_H

#include "../aliases.h"
#include "collider.h"

namespace hg::math::components {
class RectCollider : public Collider {
public:

    hg::Vec2 pos;
    hg::Vec2 size;
    bool centered = false;

    Cube getBoundingCube() const override {
        return Cube(
            entity->position() + Vec3(pos[0], pos[1], 0) - (centered ? Vec3(size[0], size[1], 0) * 0.5 : Vec3::Zero()),
            Vec3(size[0], size[1], 0)
        );
    }

    Rect getRect() const {
        auto adjSize = entity->transform.scale.resize<2>().prod(size);
        Vec2 halfSize = adjSize * 0.5f;

        // Center of the rect in local space, relative to the entity origin
        // (mirrors your original: centered rects are centered on pos, non-centered
        // treat pos as the top-left corner)
        Vec2 localCenter = centered ? pos : pos + halfSize;

        Vec2 corners[4] = {
            localCenter + Vec2(-halfSize[0], -halfSize[1]),
            localCenter + Vec2( halfSize[0], -halfSize[1]),
            localCenter + Vec2( halfSize[0],  halfSize[1]),
            localCenter + Vec2(-halfSize[0],  halfSize[1]),
        };

        Quat rot = entity->rotation();
        Vec2 worldPos = entity->position().resize<2>();

        Vec2 minP(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        Vec2 maxP(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

        for (const auto& c : corners) {
            Vec3 rotated = rot.rotatePoint(Vec3(c[0], c[1], 0));
            Vec2 p = rotated.resize<2>() + worldPos;

            minP[0] = std::min(minP[0], p[0]);
            minP[1] = std::min(minP[1], p[1]);
            maxP[0] = std::max(maxP[0], p[0]);
            maxP[1] = std::max(maxP[1], p[1]);
        }

        return Rect(minP, maxP - minP);
    }

protected:

    OBJECT_NAME(RectCollider);

};

HG_COMPONENT(Math, RectCollider)
HG_FIELD(RectCollider, hg::Vec2, pos)
HG_FIELD(RectCollider, hg::Vec2, size)
HG_FIELD(RectCollider, bool, centered)

}

#endif //HAGAME2_RECTCOLLIDER_H
