//
// Created by henry on 6/24/23.
//

#ifndef HAGAME2_ENTITYMAP_H
#define HAGAME2_ENTITYMAP_H

#include "entity.h"
#include "../utils/spatialMap.h"

namespace hg {
    class EntityMap2D {
    public:

        EntityMap2D(hg::Vec2 gridSize);

        hg::Vec2i getIndex(hg::Vec2 pos);
        hg::Vec2 getPos(hg::Vec2i idx);

        void clear();
        void insert(hg::Vec2 pos, hg::Vec2 size, hg::Entity* entity);
        std::vector<hg::Entity*> getNeighbors(hg::Vec2 pos, hg::Vec2 size);

    private:

        hg::Vec2 m_gridSize;

        hg::utils::SpatialMap2D<std::vector<hg::Entity*>, int> m_map;

    };
}



#endif //HAGAME2_ENTITYMAP_H
