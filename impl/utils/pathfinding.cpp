//
// Created by henry on 5/27/23.
//
#include "../../include/hagame/utils/pathfinding.h"


float hg::utils::PathFinding::distance(hg::Vec2i a, hg::Vec2i b) {
    if (metric == Metric::Manhatten) {
        return std::abs((a - b).sum());
    } else if (metric == Metric::Euclidean) {
        return (b.cast<float>() - a.cast<float>()).magnitude();
    }
    return 0;
}

void hg::utils::PathFinding::start(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance) {
    m_openList.clear();
    m_closedList.clear();
    m_current = nullptr;
    m_foundPath = false;

    m_start = startPos;
    m_goal = goalPos;

    auto start = std::make_shared<PathFindingNode>();
    start->position = startPos;
    start->node.position = startPos;
    start->node.cost = 0;
    start->h = distance(startPos,goalPos);
    start->g = 0;
    start->f = start->g + start->h;

    m_openList.push_back(start);
}


bool hg::utils::PathFinding::finished() {
    return m_openList.size() == 0 || (m_current != nullptr && m_current->position == m_goal);
}

void hg::utils::PathFinding::tick() {
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

std::optional<std::vector<hg::Vec2i>> hg::utils::PathFinding::search(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance) {

    start(startPos, goalPos, maxDistance);

    while (!finished()) {
        tick();
    }

    if (!m_foundPath) {
        return std::nullopt;
    }

    return constructPath(m_current.get());
}

typename hg::utils::PathFinding::PathFindingNode *hg::utils::PathFinding::getOpenNeighbor(PathFindingNode *node) {
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

void hg::utils::PathFinding::removeFromOpenList(PathFindingNode *node) {
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

bool hg::utils::PathFinding::inOpenList(PathFindingNode *node) {
    for (const auto& other : m_openList) {
        if (node->position == other->position) {
            return true;
        }
    }
    return false;
}

bool hg::utils::PathFinding::inClosedList(PathFindingNode *node) {
    for (const auto& other : m_closedList) {
        if (node->position == other->position) {
            return true;
        }
    }
    return false;
}

std::vector<hg::Vec2i> hg::utils::PathFinding::constructPath(PathFindingNode *node) {
    std::vector<hg::Vec2i> path;
    path.push_back(node->position);
    while (node->parent != nullptr) {
        node = node->parent;
        path.push_back(node->position);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::shared_ptr<typename hg::utils::PathFinding::PathFindingNode>> hg::utils::PathFinding::findNeighbors(PathFindingNode *node) {
    std::vector<std::shared_ptr<PathFindingNode>> neighbors;

    for (auto& rawNeighbor : m_neighborFn(node->node)) {
        auto neighbor = std::make_shared<PathFindingNode>();
        neighbor->position = rawNeighbor.position;
        neighbor->parent = node;
        neighbor->node = rawNeighbor;
        neighbor->position = rawNeighbor.position;
        neighbor->g = rawNeighbor.cost + node->g + distance(neighbor->position, node->position);
        neighbors.push_back(neighbor);
    }

    return neighbors;
}

