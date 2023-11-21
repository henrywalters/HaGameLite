//
// Created by henry on 11/20/23.
//

#ifndef HAGAME2_SOURCE_COMPONENT_H
#define HAGAME2_SOURCE_COMPONENT_H

#include "../player.h"
#include "../../core/component.h"

namespace hg::audio {
    class SourceComponent : public hg::Component {
    public:

        source_t source;

    protected:

        std::string toString() const override {
            return "hg::audio::SourceComponent<" + std::to_string(id()) + ">";
        }

    };
}

#endif //HAGAME2_SOURCE_H
