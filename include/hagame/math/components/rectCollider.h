//
// Created by henry on 10/15/23.
//

#ifndef HAGAME2_RECTCOLLIDER_H
#define HAGAME2_RECTCOLLIDER_H

#include "../aliases.h"
#include "../../core/component.h"

namespace hg::math::components {
class RectCollider : public hg::Component {
public:

    hg::Rect rect;

};

}

#endif //HAGAME2_RECTCOLLIDER_H
