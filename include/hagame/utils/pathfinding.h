//
// Created by henry on 5/27/23.
//

#ifndef HAGAME2_PATHFINDING_H
#define HAGAME2_PATHFINDING_H

#include <optional>
#include <vector>
#include <set>
#include <memory>

#include "../math/aliases.h"
#include "../interfaces/iNodeMap.h"
#include "../structures/binaryHeap.h"
#include "../utils/spatialMap.h"

namespace hg::utils {

    //using IsAccessible = bool;
    //using PathFindingGrid = std::vector<std::vector<IsAccessible>>;

    class PathFinding {
    public:

        enum class Metric {
            Manhatten,
            Euclidean,
        };

        Metric metric = Metric::Euclidean;

        struct Node {
            hg::Vec2i position;
            float cost;
        };

        struct PathFindingNode {
            hg::Vec2i position;
            Node node;
            std::shared_ptr<PathFindingNode> parent;
            float f, g, h;
        };

        using neighbor_f = std::function<std::vector<Node>(Node)>;

        PathFinding():
            m_neighborFn([](Node node) { return std::vector<Node>(); }),
            m_openList(structures::BinaryHeap<std::shared_ptr<PathFindingNode>>(
                    [&](auto a, auto b) { return a->f < b->f; },
                    [](auto a, auto b) { return a->position == b->position; }
            ))
        {

        }
        PathFinding(neighbor_f neighborFn):
                m_neighborFn(neighborFn),
                m_openList(structures::BinaryHeap<std::shared_ptr<PathFindingNode>>(
                        [&](auto a, auto b) { return a->f < b->f; },
                        [](auto a, auto b) { return a->position == b->position; }
                ))
        {}

        using NodePtr = std::shared_ptr<PathFindingNode>;

        std::optional<std::vector<hg::Vec2i>> search(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance = 0.0f);

        void start(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance = 0.0f);

        void tick();

        bool finished();

        bool m_foundPath = false;

        hg::Vec2i m_start;
        hg::Vec2i m_goal;
        float m_maxDistance;
        std::shared_ptr<PathFindingNode> m_current;

        neighbor_f m_neighborFn;

        structures::BinaryHeap<NodePtr> m_openList;
        SpatialMap2D<int, int> m_openListSet;
        std::set<hg::Vec2i> m_closedList;

        float distance(hg::Vec2i a, hg::Vec2i b);

        std::vector<hg::Vec2i> constructPath(NodePtr node);

        std::vector<NodePtr> findNeighbors(NodePtr node);

    };

}


#endif //HAGAME2_PATHFINDING_H
