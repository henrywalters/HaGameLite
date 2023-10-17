//
// Created by henry on 10/15/23.
//

#ifndef HAGAME2_CIRCLECOLLIDER_H
#define HAGAME2_CIRCLECOLLIDER_H

#include "../aliases.h"
#include "../../core/component.h"

namespace hg::math::components {
    class CircleCollider : public hg::Component {
    public:

        hg::Circle circle;

    };
}

#endif //HAGAME2_CIRCLECOLLIDER_H
