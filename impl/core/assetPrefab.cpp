//
// Created by henry on 8/29/26.
//
#include <hagame/core/prefab.h>
#include <hagame/core/prefabAsset.h>
#include "hagame/core/assets.h"

void hg::PrefabAsset::reload() {
    m_asset = hasPrefab(m_path) ? getPrefab(m_path) : nullptr;
}
