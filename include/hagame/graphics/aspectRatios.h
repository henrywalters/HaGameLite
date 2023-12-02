//
// Created by henry on 11/26/23.
//

#ifndef HAGAME2_ASPECTRATIOS_H
#define HAGAME2_ASPECTRATIOS_H

#include "../math/aliases.h"

namespace hg::graphics {
    struct AspectRatio {
        float numerator;
        float denominator;

        constexpr float ratio() const { return numerator / denominator; }
        constexpr float inverse() const { return denominator / numerator; }
        
        Rect getViewport(Vec2 containerSize) const {
            if (containerSize[0] > containerSize[1] * ratio()) {
                Vec2 size = Vec2(ratio() * containerSize[1], containerSize[1]);
                Vec2 pos = Vec2((containerSize[0] - size[0]) * 0.5, 0);
                return Rect(pos, size);
            } else {
                Vec2 size = Vec2(containerSize[0], inverse() * containerSize[0]);
                Vec2 pos = Vec2(0, (containerSize[1] - size[1]) * 0.5);
                return Rect(pos, size);
            }
        }

        std::string toString() const {
            return std::to_string(numerator) + ":" + std::to_string(denominator);
        }
    };

    const AspectRatio AR_1x1 = AspectRatio{1, 1};
    const AspectRatio AR_4x3 = AspectRatio{4,3};
    const AspectRatio AR_16x9 = AspectRatio{16,9};

    const std::vector<AspectRatio> ASPECT_RATIOS = {
            AR_1x1,
            AR_16x9,
            AR_4x3
    };
}

#endif //HAGAME2_ASPECTRATIOS_H
