//
// Created by henry on 8/19/26.
//

#ifndef HAGAME2_ASSET_H
#define HAGAME2_ASSET_H
#include "hagame/utils/macros.h"

#include <cstdint>

namespace hg {

    // Goes up whenever assets are loaded while the game runs (see AssetWatcher), so every Asset looks its asset up
    // again: a reloaded one may be a new object, and one that was missing may be there now
    inline std::uint64_t ASSET_GENERATION = 0;

    template <typename T>
class Asset {
    public:

        virtual ~Asset() = default;

        bool operator==(const Asset& other) { return m_path == other.m_path; }
        bool operator!=(const Asset& other) { return !(*this == other); }

        T* asset() {
            if (m_generation != ASSET_GENERATION) {
                m_generation = ASSET_GENERATION;
                if (!m_path.empty()) {
                    reload();
                }
            }
            return m_asset;
        }

        HG_GET(std::string, path);

        void path(const std::string& value) {
            if (m_path != value) {
                m_path = value;
                m_generation = ASSET_GENERATION;
                reload();
            }
        }

    protected:

        virtual void reload() = 0;

        std::string m_path;
        T* m_asset = nullptr;
        std::uint64_t m_generation = ASSET_GENERATION;

    };
}

#endif //HAGAME2_ASSET_H
