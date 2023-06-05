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

    template <typename DataType>
    class PathFinding {
    public:

        enum class Metric {
            Manhatten,
            Euclidean,
        };

        Metric metric = Metric::Euclidean;

        struct PathFindingNode {
            hg::Vec2i position;
            interfaces::INode<hg::Vec2i, DataType> data;
            float h, g, f;
            PathFindingNode *parent;
        };

        PathFinding(interfaces::INodeMap<hg::Vec2i, DataType>* nodeMap):
            m_nodeMap(nodeMap)
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

        interfaces::INodeMap<hg::Vec2i, DataType>* m_nodeMap;

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

template <typename DataType>
float hg::utils::PathFinding<DataType>::distance(hg::Vec2i a, hg::Vec2i b) {
    if (metric == Metric::Manhatten) {
        return std::abs((a - b).sum());
    } else if (metric == Metric::Euclidean) {
        return (b.cast<float>() - a.cast<float>()).magnitude();
    }
    return 0;
}

template <typename DataType>
void hg::utils::PathFinding<DataType>::start(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance) {
    m_openList.clear();
    m_closedList.clear();
    m_current = nullptr;
    m_foundPath = false;

    m_start = startPos;
    m_goal = goalPos;

    auto start = std::make_shared<PathFindingNode>();
    start->position = startPos;
    start->h = distance(startPos,goalPos);
    start->g = 0;
    start->f = start->g + start->h;

    m_openList.push_back(start);
}

template <typename DataType>
bool hg::utils::PathFinding<DataType>::finished() {
    return m_openList.size() == 0 || (m_current != nullptr && m_current->position == m_goal);
}

template <typename DataType>
void hg::utils::PathFinding<DataType>::tick() {
    m_current = m_openList[0];

    if (m_current->position == m_goal) {
        //return constructPath(current.get());
        m_foundPath = true;
        return;
    }

    removeFromOpenList(m_current.get());

    m_closedList.push_back(m_current);

    auto neighbors = findNeighbors(m_current.get());

    for (const auto& neighbor : neighbors) {
        if (inClosedList(neighbor.get())) {
            continue;
        }

        float cost = m_current->g + distance(neighbor->position, m_current->position);

        bool inOpen = inOpenList(neighbor.get());

        if (inOpen && cost < neighbor->g) {
            removeFromOpenList(neighbor.get());
        }

        if (!inOpen) {
            neighbor->g = cost;
            neighbor->h = distance(neighbor->position, m_goal);
            neighbor->f = neighbor->g + neighbor->h;
            m_openList.push_back(neighbor);
        }

    }

    std::sort(m_openList.begin(), m_openList.end(), [](const auto& a, const auto& b) {
        return a->f < b->f;
    });
}

template <typename DataType>
std::optional<std::vector<hg::Vec2i>> hg::utils::PathFinding<DataType>::search(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance) {

    start(startPos, goalPos, maxDistance);

    while (!finished()) {
        tick();
    }

    return m_foundPath ? constructPath(m_current.get()) : std::nullopt;

    m_openList.clear();
    m_closedList.clear();

    auto start = std::make_shared<PathFindingNode>();
    start->position = startPos;
    start->g = 0;
    start->f = 0;

    m_openList.push_back(start);

    while (m_openList.size() > 0) {
        auto current = m_openList[0];

        if (current->position == goalPos || distance(current->position, goalPos) < maxDistance) {
            return constructPath(current.get());
        }

        removeFromOpenList(current.get());

        m_closedList.push_back(current);

        auto neighbors = findNeighbors(current.get());

        for (const auto& neighbor : neighbors) {

            if (neighbor->g < m_current->g) {
                neighbor->f = neighbor->g + distance(neighbor->position, m_goal);
                if (!inOpenList(neighbor)) {
                    m_openList.push_back(neighbor.get());
                }
            }
            /*

            if (!inClosedList(neighbor.get())) {
                neighbor->f = neighbor->g; // + distance(neighbor->position, goalPos);
                if (!inOpenList(neighbor.get())) {
                    m_openList.push_back(neighbor);
                } else {
                    auto openNeighbor = getOpenNeighbor(neighbor.get());
                    openNeighbor->g = neighbor->g;
                    openNeighbor->parent = neighbor->parent;
                }
            }

             */
        }

        std::sort(m_openList.begin(), m_openList.end(), [](const auto& a, const auto& b) {
            return a->f < b->f;
        });
    }

    return std::nullopt;
}

template <typename DataType>
typename hg::utils::PathFinding<DataType>::PathFindingNode *hg::utils::PathFinding<DataType>::getOpenNeighbor(PathFindingNode *node) {
    int index = -1;
    for (int i = 0; i < m_openList.size(); i++) {
        if (m_openList[i]->position == node->position) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        return m_openList[index].get();
    }

    return nullptr;
}

template <typename DataType>
void hg::utils::PathFinding<DataType>::removeFromOpenList(PathFindingNode *node) {
    int index = -1;
    for (int i = 0; i < m_openList.size(); i++) {
        if (m_openList[i]->position == node->position) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        m_openList.erase(m_openList.begin() + index);
    }
}

template <typename DataType>
bool hg::utils::PathFinding<DataType>::inOpenList(PathFindingNode *node) {
    for (const auto& other : m_openList) {
        if (node->position == other->position) {
            return true;
        }
    }
    return false;
}

template <typename DataType>
bool hg::utils::PathFinding<DataType>::inClosedList(PathFindingNode *node) {
    for (const auto& other : m_closedList) {
        if (node->position == other->position) {
            return true;
        }
    }
    return false;
}

template <typename DataType>
std::vector<hg::Vec2i> hg::utils::PathFinding<DataType>::constructPath(PathFindingNode *node) {
    std::vector<hg::Vec2i> path;
    path.push_back(node->position);
    while (node->parent != nullptr) {
        node = node->parent;
        path.push_back(node->position);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template <typename DataType>
std::vector<std::shared_ptr<typename hg::utils::PathFinding<DataType>::PathFindingNode>> hg::utils::PathFinding<DataType>::findNeighbors(PathFindingNode *node) {
    std::vector<std::shared_ptr<PathFindingNode>> neighbors;

    for (const auto& rawNeighbor : m_nodeMap->getNeighbors(node->position)) {
        auto neighbor = std::make_shared<PathFindingNode>();
        neighbor->position = rawNeighbor.index;
        neighbor->parent = node;
        neighbor->data = rawNeighbor;
        neighbor->g = node->g + distance(neighbor->position, node->position);
        if (m_nodeMap->isEmpty(rawNeighbor)) {
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
}



#endif //HAGAME2_PATHFINDING_H
