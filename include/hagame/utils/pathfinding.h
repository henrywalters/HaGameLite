//
// Created by henry on 5/27/23.
//

#ifndef HAGAME2_PATHFINDING_H
#define HAGAME2_PATHFINDING_H

#include <optional>
#include <vector>
#include <memory>

#include "../math/aliases.h"
#include "../interfaces/iNodeMap.h"

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
            float h, g, f;
            PathFindingNode *parent;
        };

        using neighbor_f = std::function<std::vector<Node>(Node)>;

        PathFinding(neighbor_f neighborFn):
            m_neighborFn(neighborFn)
        {}

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

        std::vector<std::shared_ptr<PathFindingNode>> m_openList;
        std::vector<std::shared_ptr<PathFindingNode>> m_closedList;

        PathFindingNode *getOpenNeighbor(PathFindingNode *node);

        void removeFromOpenList(PathFindingNode *node);

        bool inOpenList(PathFindingNode *node);

        bool inClosedList(PathFindingNode *node);

        float distance(hg::Vec2i a, hg::Vec2i b);

        std::vector<hg::Vec2i> constructPath(PathFindingNode *node);

        std::vector<std::shared_ptr<PathFindingNode>> findNeighbors(PathFindingNode *node);

    };

}


#endif //HAGAME2_PATHFINDING_H
