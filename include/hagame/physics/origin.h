//
// Created by henry on 11/17/24.
//

#ifndef HAGAME2_ORIGIN_H
#define HAGAME2_ORIGIN_H

#include "../core/component.h"
#include "../core/entity.h"

namespace hg::physics {
    class Origin : public Component {
    public:

        hg::Vec3 origin;

    protected:

        OBJECT_NAME(Origin)

    };

    HG_COMPONENT(Physics2D, Origin)
    HG_FIELD(Origin, hg::Vec3, origin)
}

#endif //HAGAME2_ORIGIN_H
