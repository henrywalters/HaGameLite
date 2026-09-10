//
// Created by henry on 8/29/26.
//

#ifndef HAGAME2_PREFABASSET_H
#define HAGAME2_PREFABASSET_H
#include "asset.h"

namespace hg {

    class Prefab;

    class PrefabAsset : public Asset<Prefab> {
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_PREFABASSET_H
