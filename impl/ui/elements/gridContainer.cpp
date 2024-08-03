//
// Created by henry on 7/22/24.
//
#include "../../../../include/hagame/ui/elements/gridContainer.h"

using namespace hg::ui;

GridElement::GridElement(GridConfig config, hg::Vec2i index, hg::ui::Element *element):
        m_index(index),
        m_config(config),
        m_element(element)
{
    addChild(element);
}

hg::Rect hg::ui::GridElement::getRect(Rect rootRect) {
    Rect parentRect;
    if (parent()) {
        parentRect = static_cast<Element*>(parent())->getRect(rootRect);
    } else {
        parentRect = rootRect;
    }
    return adjustedRect(m_config.getRect(parentRect, m_index), style.padding, style.margin);
}

hg::ui::GridContainer::GridContainer(GridConfig config):
    m_config(config)
{

}

std::shared_ptr<GridElement> hg::ui::GridContainer::addElement(hg::ui::Element *element, hg::Vec2i index) {
    auto gridElement = std::make_shared<GridElement>(m_config, index, element);
    addChild(gridElement.get());
    return gridElement;
}

GridConfig GridConfig::Uniform(hg::Vec2i size) {
    GridConfig config;
    config.m_size = size;
    for (int h = 0; h < size[0]; h++) {
        config.m_colSizes.push_back(1.0 / size[0]);
    }
    for (int v = 0; v < size[1]; v++) {
        config.m_rowSizes.push_back(1.0 / size[1]);
    }
    return config;
}

GridConfig GridConfig::Custom(hg::Vec2i size, std::vector<double> colSizes, std::vector<double> rowSizes) {
    GridConfig config;
    config.m_size = size;

    if (colSizes.size() != size[0] || rowSizes.size() != size[1]) {
        throw std::runtime_error("Column and row sizes do not match grid size");
    }

    float colSum = 0;
    float rowSum = 0;

    for (int h = 0; h < size[0]; h++) {
        config.m_colSizes.push_back(colSizes[h]);
        colSum += colSizes[h];
    }
    for (int v = 0; v < size[1]; v++) {
        config.m_rowSizes.push_back(rowSizes[v]);
        rowSum += rowSizes[v];
    }

//    if (rowSum != 1.0 || colSum != 1.0) {
//        throw std::runtime_error("Column and row sizes do not add up to 1.0");
//    }

    return config;
}

hg::Rect GridConfig::getRect(hg::Rect container, hg::Vec2i index) {
    hg::Vec2 cellSize = container.size.prod(Vec2(m_colSizes[index[0]], m_rowSizes[index[1]]));

    float colOffset = 0.0;
    float rowOffset = 0.0;

    for (int h = 0; h < index[0]; h++) {
        colOffset += m_colSizes[h];
    }

    for (int v = 0; v < m_size[1] - index[1] - 1; v++) {
        rowOffset += m_rowSizes[v];
    }

    hg::Vec2 offset = container.size.prod(Vec2(colOffset, rowOffset));

    return Rect(container.pos + offset, cellSize);
}
