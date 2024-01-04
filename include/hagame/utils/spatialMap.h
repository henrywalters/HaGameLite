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
}

#endif //HAGAME2_SPATIALMAP_H
