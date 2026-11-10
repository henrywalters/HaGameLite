//
// Created by henry on 10/17/26.
//
#include "../../include/hagame/graphics/skeleton.h"
#include <functional>
#include <iostream>
#include "../../include/hagame/core/assets.h"

using namespace hg;
using namespace hg::graphics;

Skeleton::Skeleton(nlohmann::json const& json) {
    load(json);
}

std::optional<int> Skeleton::getBone(std::string const& name) const {
    auto it = m_boneIndex.find(name);
    if (it == m_boneIndex.end()) {
        return std::nullopt;
    }
    return it->second;
}

void Skeleton::load(nlohmann::json const& json) {
    origin = json.value("origin", "Root");

    struct Raw {
        SkeletonBone bone;
        std::string parent;
    };

    std::vector<Raw> raw;
    std::unordered_map<std::string, size_t> rawIndex;

    for (const auto& bone : json["bones"]) {
        Raw entry;
        entry.bone.name = bone["name"].get<std::string>();
        entry.bone.joint = bone["joint"].get<std::string>();
        entry.bone.part = bone.contains("part") && !bone["part"].is_null() ? bone["part"].get<std::string>() : "";
        entry.parent = bone.contains("parent") && !bone["parent"].is_null() ? bone["parent"].get<std::string>() : "";

        if (rawIndex.find(entry.bone.name) != rawIndex.end()) {
            std::cout << "WARNING: Skeleton has two bones named " << entry.bone.name << ", ignoring the second\n";
            continue;
        }
        if (!entry.parent.empty() && entry.parent == entry.bone.name) {
            std::cout << "WARNING: Skeleton bone " << entry.bone.name << " is its own parent, making it a root\n";
            entry.parent = "";
        }

        rawIndex[entry.bone.name] = raw.size();
        raw.push_back(entry);
    }

    for (auto& entry : raw) {
        if (!entry.parent.empty() && rawIndex.find(entry.parent) == rawIndex.end()) {
            std::cout << "WARNING: Skeleton bone " << entry.bone.name << " has unknown parent " << entry.parent << ", making it a root\n";
            entry.parent = "";
        }
    }

    // Order bones so parents come first, letting the rig walk them once to build every bone's transform
    bones.clear();
    m_boneIndex.clear();
    std::vector<int> state(raw.size(), 0); // 0 = unvisited, 1 = visiting, 2 = placed

    std::function<void(size_t)> place = [&](size_t i) {
        if (state[i] == 2) return;
        if (state[i] == 1) {
            std::cout << "WARNING: Skeleton bone " << raw[i].bone.name << " is part of a parent loop, making it a root\n";
            raw[i].parent = "";
        }
        state[i] = 1;
        if (!raw[i].parent.empty()) {
            place(rawIndex[raw[i].parent]);
            if (state[i] == 2) return; // Placed while breaking a parent loop through it
        }
        auto bone = raw[i].bone;
        bone.parent = raw[i].parent.empty() ? -1 : m_boneIndex[raw[i].parent];
        m_boneIndex[bone.name] = bones.size();
        bones.push_back(bone);
        state[i] = 2;
    };

    for (size_t i = 0; i < raw.size(); i++) {
        place(i);
    }
}

void SkeletonAsset::reload() {
    m_asset = hasSkeleton(m_path) ? getSkeleton(m_path) : nullptr;
}
