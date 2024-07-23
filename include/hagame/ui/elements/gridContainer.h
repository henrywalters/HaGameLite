//
// Created by henry on 7/22/24.
//

#ifndef HAGAME2_GRIDCONTAINER_H
#define HAGAME2_GRIDCONTAINER_H

#include "../element.h"
#include "../../utils/macros.h"

namespace hg::ui {

    inline std::vector<double> uniformSize(int N) {
        std::vector<double> out;
        for (int i = 0; i < N; i++) {
            out.push_back(1. / N);
        }
        return out;
    }

    class GridConfig {
    public:

        static GridConfig Uniform(hg::Vec2i size);
        static GridConfig Custom(hg::Vec2i size, std::vector<double> colSizes, std::vector<double> rowSizes);

        Rect getRect(Rect container, hg::Vec2i index);

    private:

        GridConfig() {}

        hg::Vec2i m_size;
        std::vector<double> m_colSizes;
        std::vector<double> m_rowSizes;

    };

    class GridElement : public Element {
    public:

        GridElement(GridConfig config, hg::Vec2i index, Element* element);

        Rect getRect(Rect rootRect);

        HG_GET_PTR(Element, element)

    private:

        GridConfig m_config;
        hg::Vec2i m_index;
        Element* m_element;
    };

    class GridContainer : public Element {
    public:

        GridContainer(GridConfig config);

        void addElement(Element* element, hg::Vec2i index);

    private:
        GridConfig m_config;
    };
}

#endif //HAGAME2_GRIDCONTAINER_H
