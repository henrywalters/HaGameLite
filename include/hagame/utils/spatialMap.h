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
    class SpatialMap2D : public hg::interfaces::INodeMap<hg::math::Vector<2, SpatialType>, std::vector<DataType>> {
    public:

        using SpatialVector = hg::math::Vector<2, SpatialType>;

        using Node = hg::interfaces::INode<hg::math::Vector<2, SpatialType>, std::vector<DataType>>;

        void clear() override {
            m_map.clear();
        }

        void clear(SpatialVector position) {
            if (m_map.find(position[0]) != m_map.end() && m_map[position[0]].find(position[1]) != m_map[position[0]].end()) {
                m_size -= get(position).value.size();
                m_map[position[0]].erase(position[1]);
            }
        }

        void set(SpatialVector position, std::vector<DataType> value) override {
            initialize(position);
            m_map[position[0]][position[1]] = value;
            m_size += value.size();
        }

        void insert(SpatialVector position, DataType value) {
            initialize(position);
            m_map[position[0]][position[1]].push_back(value);
            m_size++;
        }

        Node get(SpatialVector position) override {
            initialize(position);
            Node node;
            node.value = m_map[position[0]][position[1]];
            node.index = position;
            return node;
        }

        std::vector<Node> getNeighbors(SpatialVector position) override {
            std::vector<Node> nodes;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {
                        continue;
                    }
                    nodes.push_back(get(position + SpatialVector(i, j)));
                }
            }

            return nodes;
        }

        bool isEmpty(Node node) override {
            return node.value.size() == 0;
        }

        bool has(SpatialVector position) override {
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

        void initialize(SpatialVector position) {
            if (m_map.find(position[0]) == m_map.end()) {
                m_map.insert(std::make_pair(position[0], std::unordered_map<SpatialType, std::vector<DataType>>()));
            }
            if (m_map[position[0]].find(position[1]) == m_map[position[0]].end()) {
                m_map[position[0]].insert(std::make_pair(position[1], std::vector<DataType>()));
            }
        }
    };
}

#endif //HAGAME2_SPATIALMAP_H
