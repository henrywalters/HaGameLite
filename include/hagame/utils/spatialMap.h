//
// Created by henry on 4/15/23.
//

#ifndef HAGAME2_SPATIALMAP_H
#define HAGAME2_SPATIALMAP_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "../interfaces/iNodeMap.h"
#include "../math/aliases.h"

namespace hg::utils {
    template <typename DataType, typename SpatialType>
    class SpatialMap2D : public hg::interfaces::INodeMap<hg::math::Vector<2, SpatialType>, DataType> {
    public:

        using SpatialVector = hg::math::Vector<2, SpatialType>;

        using Node = hg::interfaces::INode<hg::math::Vector<2, SpatialType>, DataType>;

        void clear() override {
            m_map.clear();
        }

        void set(SpatialVector position, DataType value) override {
            initialize(position);
            m_map[position[0]][position[1]] = value;
            m_size += 1;

            for (int i = 0; i < 2; i++) {
                if (position[i] < m_min[i]) {
                    m_min[i] = position[i];
                }
                if (position[i] > m_max[i]) {
                    m_max[i] = position[i];
                }
            }
        }

        void remove(SpatialVector position) {
            initialize(position);
            m_map[position[0]].erase(position[1]);
            m_size -= 1;
        }

        Node get(SpatialVector position) override {
            initialize(position);
            Node node;
            node.value = m_map[position[0]][position[1]];
            node.index = position;
            return node;
        }

        DataType* getRef(SpatialVector position) {
            return &m_map[position[0]][position[1]];
        }

        std::vector<hg::Vec2i> getNeighbors(SpatialVector position, bool allowDiagnols = true)  {
            std::vector<hg::Vec2i> nodes;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {
                        continue;
                    }

                    if (!allowDiagnols && std::abs(i) == 1 && std::abs(j) == 1) {
                        continue;
                    }

                    hg::Vec2i index = position + SpatialVector(i, j);
                    nodes.push_back(index);

                }
            }

            return nodes;
        }

        bool has(SpatialVector position) override {
            return m_map.find(position[0]) != m_map.end() && m_map[position[0]].find(position[1]) != m_map[position[0]].end();
        }

        size_t size() const { return m_size; }

        void forEach(std::function<void(hg::Vec2i, DataType)> fn) {
            for (const auto&[col, map] : m_map) {
                for (const auto&[row, val] : m_map[col]) {
                    fn(hg::Vec2i(col, row), val);
                }
            }
        }

        SpatialVector min() const { return m_min; }
        SpatialVector max() const { return m_max; }

        std::vector<hg::Rect> getRectangles(hg::Vec2 cellSize);

    private:

        std::unordered_map<SpatialType, std::unordered_map<SpatialType, DataType>> m_map;

        SpatialVector m_min;
        SpatialVector m_max;

        size_t m_size;

        void initialize(SpatialVector position) {
            if (m_map.find(position[0]) == m_map.end()) {
                m_map.insert(std::make_pair(position[0], std::unordered_map<SpatialType, DataType>()));
            }
        }
    };

    template<typename DataType, typename SpatialType>
    std::vector<hg::Rect> SpatialMap2D<DataType, SpatialType>::getRectangles(hg::Vec2 cellSize) {

        std::vector<hg::Rect> rects;

        hg::utils::SpatialMap2D<bool, int> accountedFor;

        std::function<bool(Vec2i)> unaccountedFor = [&](Vec2i index) -> bool {
            return has(index) && !accountedFor.has(index);
        };

        std::function<Vec2i(Vec2i)> getBottomLeftIndex = [&](Vec2i index) -> Vec2i {
            bool canMoveLeft = unaccountedFor(index - Vec2i(1, 0));
            bool canMoveDown = unaccountedFor(index - Vec2i(0, 1));

            while (canMoveLeft || canMoveDown) {

                bool below = unaccountedFor(index - Vec2i(0, 1));
                bool left = unaccountedFor(index - Vec2i(1, 0));
                bool belowLeft = unaccountedFor(index - Vec2i(1, 1));

                if (canMoveLeft && canMoveDown) {

                    if (below && left && belowLeft) {
                        index -= Vec2i(1, 1);
                    } else if (below && !(left || belowLeft)) {
                        canMoveLeft = false;
                        index -= Vec2i(0, 1);
                    } else if (left && (!below || belowLeft)) {
                        canMoveDown = false;
                        index -= Vec2i(1, 0);
                    } else {
                        canMoveLeft = false;
                        canMoveDown = false;
                    }
                } else if (canMoveLeft) {
                    if (left) {
                        index -= Vec2i(1, 0);
                    } else {
                        canMoveLeft = false;
                    }
                } else {
                    if (below) {
                        index -= Vec2i(0, 1);
                    } else {
                        canMoveDown = false;
                    }
                }
            }

            return index;
        };

        std::function<void(Recti&, Vec2i)> expandRect = [&] (Recti& rect, Vec2i index) -> void {
            bool canMoveRight = unaccountedFor(index + Vec2i(1, 0));
            bool canMoveUp = unaccountedFor(index + Vec2i(0, 1));

            while (canMoveRight || canMoveUp) {

                if (canMoveRight) {
                    bool isRect = true;
                    for (int y = 0; y <= rect.size[1]; y++) {
                        if (!has(index + rect.size + Vec2i(1, y))) {
                            isRect = false;
                            break;
                        }
                    }
                    if (isRect) {
                        for (int y = 0; y <= rect.size[1]; y++) {
                            accountedFor.set(index + rect.size + Vec2i(1, y), true);
                        }
                        rect.size[0] += 1;
                    } else {
                        canMoveRight = false;
                    }
                }

                if (canMoveUp) {
                    bool isRect = true;
                    for (int x = 0; x <= rect.size[0]; x++) {
                        if (!has(index + rect.size  + Vec2i(x, 1))) {
                            isRect = false;
                            break;
                        }
                    }
                    if (isRect) {
                        for (int x = 0; x <= rect.size[0]; x++) {
                            accountedFor.set(index + rect.size + Vec2i(x, 1), true);
                        }
                        rect.size[1] += 1;
                    } else {
                        canMoveUp = false;
                    }
                }

            }
        };

        std::function<void(Recti&, Vec2i)> expandRectV2 = [&] (Recti& rect, Vec2i index) -> void {

            bool canMoveRight = unaccountedFor(index + Vec2i(1, 0));
            bool canMoveUp = unaccountedFor(index + Vec2i(0, 1));

            while (canMoveRight || canMoveUp) {

                if (canMoveRight && canMoveUp) { // Can expand as a perfect square
                    for (int i = 0; i < rect.size[1] + 1; i++) {
                        if (!has(index + Vec2i(rect.size[0], 0) + Vec2i(1, i))) {
                            canMoveRight = false;
                        }
                        if (!has(index + Vec2i(0, rect.size[1]) + Vec2i(i, 1))) {
                            canMoveUp = false;
                        }
                    }
                    if (!has(index + Vec2i(rect.size[0], rect.size[1]) + Vec2i(1, 1))) { // If can't go to the corner piece, sacrifice going up to move right
                        canMoveUp = false;
                    }
                    if (canMoveRight && canMoveUp) {
                        for (int i = 0; i <= rect.size[1] + 1; i++) {
                            accountedFor.set(index + Vec2i(rect.size[0], 0) + Vec2i(1, i), true);
                            accountedFor.set(index + Vec2i(0, rect.size[1]) + Vec2i(i, 1), true);
                        }
                        rect.size += hg::Vec2i(1);
                    }
                } else if (canMoveRight) { // Can only move right
                    for (int i = 0; i <= rect.size[1]; i++) {
                        if (!has(index + Vec2i(rect.size[0], 0) + Vec2i(1, i))) {
                            canMoveRight = false;
                        }
                    }

                    if (canMoveRight) {
                        for (int i = 0; i <= rect.size[1]; i++) {
                            accountedFor.set(index + Vec2i(rect.size[0], 0) + Vec2i(1, i), true);
                        }
                        rect.size[0] += 1;
                    }
                } else if (canMoveUp) {
                    for (int i = 0; i <= rect.size[0]; i++) {
                        if (!has(index + Vec2i(0, rect.size[1]) + Vec2i(i, 1))) {
                            canMoveUp = false;
                        }
                    }
                    if (canMoveUp) {
                        for (int i = 0; i <= rect.size[0]; i++) {
                            accountedFor.set(index + Vec2i(0, rect.size[1]) + Vec2i(i, 1), true);
                        }
                        rect.size[1] += 1;
                    }
                }
            }
        };

        forEach([&](auto index, auto tile) {
            if (unaccountedFor(index)) {
                auto startIdx = getBottomLeftIndex(index);
                if (unaccountedFor(startIdx)) {
                    Recti rect(startIdx, Vec2i::Zero());
                    expandRectV2(rect, startIdx);
                    accountedFor.set(startIdx, true);
                    auto bottomLeft = rect.pos.cast<float>().prod(cellSize);
                    auto topRight = (rect.pos + rect.size).cast<float>().prod(cellSize);
                    auto size = topRight - bottomLeft;
                    Rect r(bottomLeft, size + cellSize);
                    rects.push_back(r);

                    if (!r.contains(index.template cast<float>() + cellSize * 0.5)) {
                        rects.emplace_back(index.template cast<float>().prod(cellSize), cellSize);
                    }
                }
            }
        });

        return rects;
    }
}

#endif //HAGAME2_SPATIALMAP_H
