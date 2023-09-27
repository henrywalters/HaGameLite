//
// Created by henry on 5/27/23.
//
#include "../../include/hagame/utils/pathfinding.h"
#include "../../include/hagame/utils/profiler.h"


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
    m_openListSet.clear();
    m_current = nullptr;
    m_foundPath = false;

    m_start = startPos;
    m_goal = goalPos;

    auto start = std::make_shared<PathFindingNode>();
    start->position = startPos;
    start->node.position = startPos;
    start->node.cost = 0;
    float h = distance(startPos,goalPos);

    m_openList.insert(start);
    m_openListSet.set(start->position, 1);
}


bool hg::utils::PathFinding::finished() {
    return m_openList.size() == 0 || (m_current != nullptr && m_current->position == m_goal);
}

void hg::utils::PathFinding::tick() {

    m_current = m_openList.getTop();
    m_openList.deleteTop();


    if (m_current->position == m_goal) {
        //return constructPath(current.get());
        m_foundPath = true;
        return;
    }

    m_closedList.insert(m_current->position);

    auto neighbors = findNeighbors(m_current);

    for (const auto& neighbor : neighbors) {
        if (m_closedList.contains(neighbor->position)) {
            continue;
        }

        bool inOpen = m_openListSet.has(neighbor->position);

        float cost = m_current->g + distance(neighbor->position, m_current->position);

        if (inOpen && cost < neighbor->g) {
            m_openListSet.remove(neighbor->position);
        }

        if (!inOpen) {
            neighbor->g = cost;
            neighbor->h = distance(neighbor->position, m_goal);
            neighbor->f = neighbor->g + neighbor->h;
            m_openList.insert(neighbor);
            m_openListSet.set(neighbor->position, 1);
        }
    }
}

std::optional<std::vector<hg::Vec2i>> hg::utils::PathFinding::search(hg::Vec2i startPos, hg::Vec2i goalPos, float maxDistance) {

    hg::utils::Profiler::Start();

    start(startPos, goalPos, maxDistance);

    while (!finished()) {
        tick();
    }

    if (!m_foundPath) {
        return std::nullopt;
    }

    hg::utils::Profiler::End();

    return constructPath(m_current);
}

std::vector<hg::Vec2i> hg::utils::PathFinding::constructPath(NodePtr node) {
    std::vector<hg::Vec2i> path;
    path.push_back(node->position);
    while (node->parent != nullptr) {
        node = node->parent;
        path.push_back(node->position);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::shared_ptr<typename hg::utils::PathFinding::PathFindingNode>> hg::utils::PathFinding::findNeighbors(NodePtr node) {
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

