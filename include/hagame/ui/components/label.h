//
// Created by henry on 9/2/25.
//

#ifndef HAGAME2_LABEL_COMPONENT_H
#define HAGAME2_LABEL_COMPONENT_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>

namespace hg::ui::components {
    class Label : public hg::Component {
    public:

        std::string font;
        std::string text;

    protected:

        OBJECT_NAME(Label)
    };

    HG_COMPONENT(UI, Label)
    HG_FIELD(Label, std::string, font)
    HG_FIELD(Label, std::string, text)
}


#endif //HAGAME2_LABEL_H
