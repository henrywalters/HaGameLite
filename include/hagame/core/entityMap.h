//
// Created by henry on 6/24/23.
//

#ifndef HAGAME2_ENTITYMAP_H
#define HAGAME2_ENTITYMAP_H

#include "entity.h"
#include "../math/collisions.h"
#include "../utils/spatialMap.h"

namespace hg {
    class EntityMap2D {
    public:

        EntityMap2D(hg::Vec2 gridSize);

        HG_GET(size_t, size);

        hg::Vec2i getIndex(hg::Vec2 pos);
        hg::Vec2 getPos(hg::Vec2i idx);

        void clear();
        void insert(hg::Vec2 pos, hg::Vec2 size, hg::Entity* entity);
        void remove(hg::Vec2 pos, hg::Vec2 size, hg::Entity* entity);
        std::vector<hg::Entity*> getNeighbors(hg::Vec2 pos, hg::Vec2 size);
        std::optional<hg::Entity*> raycast(hg::math::Ray ray, float& t, std::vector<Entity*> ignore = {});

    private:

        hg::Vec2 m_gridSize;
        size_t m_size;

        hg::utils::SpatialMap2D<std::vector<hg::Entity*>, int> m_map;

    };
}



#endif //HAGAME2_ENTITYMAP_H
