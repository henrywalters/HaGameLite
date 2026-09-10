//
// Created by henry on 8/19/26.
//

#ifndef HAGAME2_ASSET_H
#define HAGAME2_ASSET_H
#include "hagame/utils/macros.h"

namespace hg {
    template <typename T>
class Asset {
    public:

        bool operator==(const Asset& other) { return m_path == other.m_path; }
        bool operator!=(const Asset& other) { return !(*this == other); }

        HG_GET_PTR(T, asset);
        HG_GET_SET(std::string, path, reload);

    protected:

        virtual void reload() = 0;

        std::string m_path;
        T* m_asset = nullptr;

    };
}

#endif //HAGAME2_ASSET_H
