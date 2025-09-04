//
// Created by henry on 3/25/23.
//

#ifndef HAGAME2_LINE_COMPONENT_H
#define HAGAME2_LINE_COMPONENT_H

#include "../../core/component.h"
#include "../../core/entity.h"
#include "../primitives/line.h"
#include "../color.h"
#include "../mesh.h"
#include "../../math/aliases.h"

namespace hg::graphics {
    class Line : public Component {
    public:

        primitives::Line line;

    protected:

        OBJECT_NAME(Line)

    };

    HG_COMPONENT(Graphics, Line)

}

#endif //HAGAME2_LINE_COMPONENT_H
