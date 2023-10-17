//
// Created by henry on 6/24/23.
//

#include "../../include/hagame/core/entityMap.h"
#include "../../include/hagame/math/functions.h"

using namespace hg;

EntityMap2D::EntityMap2D(hg::Vec2 gridSize):
    m_gridSize(gridSize)
{}

hg::Vec2i EntityMap2D::getIndex(hg::Vec2 pos) {
    return pos.div(m_gridSize).floor().cast<int>();
}

hg::Vec2 EntityMap2D::getPos(hg::Vec2i idx) {
    return idx.cast<float>().prod(m_gridSize);
}

void EntityMap2D::clear() {
    m_map.clear();
    m_size = 0;
}

void EntityMap2D::insert(hg::Vec2 pos, hg::Vec2 size, hg::Entity *entity) {
    m_size++;
    hg::Vec2i startIdx = getIndex(pos + size * -0.5);
    hg::Vec2i endIdx = getIndex(pos + size * 0.5);

    for (int i = startIdx[0]; i <= endIdx[0]; i++) {
       for (int j = startIdx[1]; j <= endIdx[1]; j++) {
           hg::Vec2i index(i, j);
           auto list = m_map.getRef(index);
           if (std::find(list->begin(), list->end(), entity) == list->end()) {
               list->push_back(entity);
           }
       }
    }
}

std::vector<hg::Entity *> EntityMap2D::getNeighbors(hg::Vec2 pos, hg::Vec2 size) {
    std::vector<hg::Entity*> neighbors;
    hg::Vec2i startIdx = getIndex(pos + size * -0.5);
    hg::Vec2i endIdx = getIndex(pos + size * 0.5);

    for (int i = startIdx[0]; i <= endIdx[0]; i++) {
        for (int j = startIdx[1]; j <= endIdx[1]; j++) {
            hg::Vec2i index(i, j);
            for (const auto& neighbor : m_map.get(index).value) {
                if (std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end()) {
                    neighbors.push_back(neighbor);
                }
            }
        }
    }

    return neighbors;
}

std::optional<hg::math::collisions::Hit> EntityMap2D::raycast(hg::math::Ray ray, float& t) {
    Vec2i startIdx = getIndex(ray.origin.resize<2>());
    Vec2i endIdx = getIndex((ray.origin + ray.direction).resize<2>());

    float minT;
    float tmpT;
    bool hasMinT = false;
    std::optional<math::collisions::Hit> hit;


    for (const auto& index : bresenham(startIdx, endIdx)) {
        for (const auto& entity : m_map.get(index).value) {
            auto tmpHit = math::collisions::checkRayAgainstEntity(ray, entity, tmpT);
            if (tmpHit.has_value() && (!hasMinT || tmpT < minT)) {
                hasMinT = true;
                minT = tmpT;
                hit = tmpHit;
            }
        }
    }

    if (hasMinT) {
        t = minT;
        return hit;
    }

    return std::nullopt;
}




