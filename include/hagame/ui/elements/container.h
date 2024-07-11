//
// Created by henry on 2/22/24.
//

#ifndef HAGAME2_CONTAINER_H
#define HAGAME2_CONTAINER_H

#include "../element.h"

namespace hg::ui {
    class Container : public Element {

    public:

        bool wrapIndex = false;

    protected:

        bool onTrigger(utils::enum_t event) override {
            if (event == UITriggers::Previous) {
                m_index--;
                if (m_index == -1) {
                    m_index = wrapIndex ? children().size() - 1 : 0;
                }
                ((Element*) children()[m_index])->trigger(UITriggers::Focus);
                return true;
            }

            if (event == UITriggers::Next) {
                m_index++;
                if (m_index == children().size()) {
                    m_index = wrapIndex ? 0 : children().size() - 1;
                }
                ((Element*) children()[m_index])->trigger(UITriggers::Focus);
                return true;
            }

            return false;
        }

    private:

        int m_index = 0;

    };
}

#endif //HAGAME2_CONTAINER_H
