//
// Created by henry on 10/17/26.
//

#ifndef HAGAME2_RIGANIMATION_H
#define HAGAME2_RIGANIMATION_H

#include <map>
#include <optional>
#include "../core/asset.h"
#include "../math/aliases.h"
#include "nlohmann/json.hpp"

namespace hg::graphics {

    // How a key blends into the next one
    enum class RigEase {
        Linear,
        InOut,
    };

    template <typename T>
    struct RigKey {
        float time;
        T value;
        RigEase ease = RigEase::Linear;
    };

    // The animated values of one bone. Rotation is in degrees, offset in units, both relative to the rest pose
    struct RigBoneTrack {
        std::vector<RigKey<float>> rotation;
        std::vector<RigKey<Vec2>> offset;

        bool empty() const { return rotation.empty() && offset.empty(); }
    };

    struct RigClipEvent {
        float time;
        std::string name;
    };

    struct RigClip {
        float duration = 1.0f;
        bool loop = true;

        // Keyed by bone name. Bones without a track are left alone by the clip
        std::map<std::string, RigBoneTrack> tracks;

        // Sorted by time
        std::vector<RigClipEvent> events;

        // Values at a time in seconds. Looping clips wrap the time and blend from their last key back to their first
        std::optional<float> sampleRotation(std::string const& bone, float time) const;
        std::optional<Vec2> sampleOffset(std::string const& bone, float time) const;

        // Adds a key, replacing the value of any key at the same time. Without an ease, a replaced key keeps its ease
        // and a new key is linear
        void setRotationKey(std::string const& bone, float time, float degrees, std::optional<RigEase> ease = std::nullopt);
        void setOffsetKey(std::string const& bone, float time, Vec2 offset, std::optional<RigEase> ease = std::nullopt);

        // Removes every key at a time, from every bone
        void removeKeysAt(float time);

        // Every time with a key on any bone, sorted
        std::vector<float> keyTimes() const;

        void addEvent(float time, std::string const& name);

        // Wraps a time into the clip for looping clips, clamps it otherwise
        float wrap(float time) const;
    };

    // The clips of one skeleton (.hg_anims): { "clips": { "idle": { "duration", "loop", "tracks", "events" } } }
    class RigAnimationSet {
    public:

        // Keys closer than this many seconds are the same key
        static constexpr float KEY_EPSILON = 0.0005f;

        RigAnimationSet(nlohmann::json const& json);

        // The file this set was loaded from, so editors can save back to it
        std::string filePath;

        std::map<std::string, RigClip> clips;

        RigClip* getClip(std::string const& name);

        void load(nlohmann::json const& json);
        nlohmann::json save() const;

        // Writes the set back to filePath. False if it has no file
        bool saveToFile() const;
    };

    class RigAnimationSetAsset : public Asset<RigAnimationSet> {
    public:
        using Asset::Asset;
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_RIGANIMATION_H
