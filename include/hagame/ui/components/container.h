//
// Created by henry on 9/2/25.
//

#ifndef HAGAME2_CONTAINER_COMPONENT_H
#define HAGAME2_CONTAINER_COMPONENT_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>

namespace hg::ui::components {
class Container : public hg::Component {
public:

protected:

    OBJECT_NAME(Container)
};

HG_COMPONENT(UI, Container)
}

#endif //HAGAME2_CONTAINER_H
