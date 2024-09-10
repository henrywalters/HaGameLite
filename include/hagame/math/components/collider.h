//
// Created by henry on 10/24/23.
//

#ifndef HAGAME2_COLLIDER_H
#define HAGAME2_COLLIDER_H

#include "../aliases.h"
#include "../../core/component.h"
#include "../../core/entity.h"

namespace hg::math::components {
    class Collider : public hg::Component {
    public:

        virtual Cube getBoundingCube() const = 0;
        static Collider* FromEntity(Entity* entity);
    };

}

#endif //HAGAME2_COLLIDER_H
