//
// Created by henry on 10/17/26.
//

#ifndef HAGAME2_SKELETON_H
#define HAGAME2_SKELETON_H

#include <optional>
#include <unordered_map>
#include "../core/asset.h"
#include "nlohmann/json.hpp"

namespace hg::graphics {

    struct SkeletonBone {
        std::string name;
        int parent = -1; // Index into the skeleton's bones, -1 for a root bone
        std::string joint; // The rig atlas joint this bone rotates around
        std::string part; // The rig atlas part drawn by this bone, empty for none
    };

    // The bone hierarchy of a rig (.hg_skel). Bones refer to joints and parts of a RigAtlas by name, so any atlas
    // that uses the same names can share a skeleton:
    // {
    //   "origin": "Root",   the joint placed at the entity's position
    //   "bones": [ { "name": "torso", "parent": "hip", "joint": "Hip", "part": "Torso" } ]
    // }
    class Skeleton {
    public:

        Skeleton(nlohmann::json const& json);

        std::string origin;

        // Parents always come before their children
        std::vector<SkeletonBone> bones;

        std::optional<int> getBone(std::string const& name) const;

        void load(nlohmann::json const& json);

    private:

        std::unordered_map<std::string, int> m_boneIndex;
    };

    class SkeletonAsset : public Asset<Skeleton> {
    public:
        using Asset::Asset;
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_SKELETON_H
