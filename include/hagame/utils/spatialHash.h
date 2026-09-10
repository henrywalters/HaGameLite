//
// Created by henry on 7/29/26.
//

#ifndef HAGAME2_SPATIALHASH_H
#define HAGAME2_SPATIALHASH_H

#include <unordered_set>
#include <unordered_map>

#include "../math/vector.h"
// #include "serialization.h"


namespace hg::utils {
    template <typename T>
    class SpatialHash2D {
    public:
        using SpatialVector = hg::math::Vector<2, T>;

        void clear() {
            m_hash.clear();
        }

        bool empty() const {
            return m_hash.empty();
        }

        void add(SpatialVector const& pos) {
            if (m_hash.find(pos.x()) == m_hash.end()) {
                m_hash.insert(std::make_pair(pos.x(), std::unordered_set<T>()));
            }

            m_hash.at(pos.x()).insert(pos.y());
        }

        void remove(SpatialVector const& pos) {
            if (m_hash.find(pos.x()) == m_hash.end()) {
                return;
            }

            m_hash.at(pos.x()).erase(pos.y());
        }

        bool has(SpatialVector const& pos) const {
            return m_hash.find(pos.x()) != m_hash.end() && m_hash.at(pos.x()).find(pos.y()) != m_hash.at(pos.x()).end();
        }

        void forEach(std::function<void(SpatialVector)> fn) const {
            for (const auto&[col, rowMap] : m_hash) {
                for (const auto& row : rowMap) {
                    fn(hg::Vec2i(col, row));
                }
            }
        }

    private:

        std::unordered_map<T, std::unordered_set<T>> m_hash;
    };

    template <typename T>
    inline void to_json(nlohmann::json& j, SpatialHash2D<T> const& tilemap) {
        j = nlohmann::json::array();
        tilemap.forEach([&j](hg::math::Vector<2, T> const& idx) {
            j.push_back(idx);
        });
    }

    template <typename T>
    inline void from_json(nlohmann::json const& j, SpatialHash2D<T>& tilemap) {
        for (auto const& entry : j) {
            tilemap.add(entry.get<hg::math::Vector<2, T>>());
        }
    }
}

#endif //HAGAME2_SPATIALHASH_H
