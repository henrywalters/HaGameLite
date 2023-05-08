//
// Created by henry on 4/15/23.
//

#ifndef HAGAME2_SPATIALMAP_H
#define HAGAME2_SPATIALMAP_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "../math/aliases.h"

namespace hg::utils {
    template <typename DataType, typename SpatialType>
    class SpatialMap2D {
    public:

        using SpatialVector = hg::math::Vector<2, SpatialType>;

        void clear() {
            m_map.clear();
        }

        void clear(SpatialVector position) {
            if (m_map.find(position[0]) != m_map.end() && m_map[position[0]].find(position[1]) != m_map[position[0]].end()) {
                m_size -= get(position).size();
                m_map[position[0]].erase(position[1]);
            }
        }

        void insert(SpatialVector position, DataType value) {
            if (m_map.find(position[0]) == m_map.end()) {
                m_map.insert(std::make_pair(position[0], std::unordered_map<SpatialType, std::vector<DataType>>()));
            }
            if (m_map[position[0]].find(position[1]) == m_map[position[0]].end()) {
                m_map[position[0]].insert(std::make_pair(position[1], std::vector<DataType>()));
            }

            m_map[position[0]][position[1]].push_back(value);
            m_size++;
        }

        std::vector<DataType>& get(SpatialVector position) {
            return m_map[position[0]][position[1]];
        }

        bool has(SpatialVector position) {
            return m_map.find(position[0]) != m_map.end() && m_map[position[0]].find(position[1]) != m_map[position[0]].end();
        }

        size_t size() const { return m_size; }

        void forEach(std::function<void(hg::Vec2i, DataType)> fn) {
            for (const auto&[col, map] : m_map) {
                for (const auto&[row, tiles] : m_map[col]) {
                    for (const auto& val : m_map[col][row]) {
                        fn(hg::Vec2i(col, row), val);
                    }
                }
            }
        }
    private:

        std::unordered_map<SpatialType, std::unordered_map<SpatialType, std::vector<DataType>>> m_map;

        size_t m_size;

    };
}

#endif //HAGAME2_SPATIALMAP_H
