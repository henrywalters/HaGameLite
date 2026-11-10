//
// Created by henry on 10/17/26.
//
#include "../../include/hagame/graphics/rigAnimation.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include "../../include/hagame/core/assets.h"

using namespace hg;
using namespace hg::graphics;

namespace {

    float applyEase(RigEase ease, float t) {
        switch (ease) {
            case RigEase::InOut:
                return t * t * (3.0f - 2.0f * t);
            default:
                return t;
        }
    }

    // Saved values are rounded so files stay readable and diff cleanly
    double rounded(float value) { return std::round(value * 10000.0) / 10000.0; }

    float lerp(float a, float b, float t) { return a + (b - a) * t; }
    Vec2 lerp(Vec2 a, Vec2 b, float t) { return a + (b - a) * t; }

    // Samples keys sorted by time. Looping clips treat the keys as repeating every duration
    template <typename T>
    std::optional<T> sample(std::vector<RigKey<T>> const& keys, float time, float duration, bool loop) {
        if (keys.empty()) {
            return std::nullopt;
        }
        if (keys.size() == 1) {
            return keys[0].value;
        }

        auto& first = keys.front();
        auto& last = keys.back();

        if (time < first.time || time >= last.time) {
            if (!loop || duration <= 0.0f) {
                return time < first.time ? first.value : last.value;
            }
            // Between the last key and the first key of the next loop
            float span = (first.time + duration) - last.time;
            if (span <= 0.0f) {
                return last.value;
            }
            float into = time >= last.time ? time - last.time : time + duration - last.time;
            return lerp(last.value, first.value, applyEase(last.ease, into / span));
        }

        for (size_t i = 0; i + 1 < keys.size(); i++) {
            auto& a = keys[i];
            auto& b = keys[i + 1];
            if (time >= a.time && time < b.time) {
                float t = b.time > a.time ? (time - a.time) / (b.time - a.time) : 1.0f;
                return lerp(a.value, b.value, applyEase(a.ease, t));
            }
        }

        return last.value;
    }

    template <typename T>
    void setKey(std::vector<RigKey<T>>& keys, float time, T value, std::optional<RigEase> ease) {
        for (auto& key : keys) {
            if (std::abs(key.time - time) < RigAnimationSet::KEY_EPSILON) {
                key.value = value;
                if (ease.has_value()) {
                    key.ease = ease.value();
                }
                return;
            }
        }
        keys.push_back(RigKey<T>{time, value, ease.value_or(RigEase::Linear)});
        std::sort(keys.begin(), keys.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
    }

    template <typename T>
    void removeKeyAt(std::vector<RigKey<T>>& keys, float time) {
        std::erase_if(keys, [time](auto const& key) { return std::abs(key.time - time) < RigAnimationSet::KEY_EPSILON; });
    }

    std::string easeName(RigEase ease) {
        return ease == RigEase::InOut ? "inOut" : "linear";
    }

    RigEase easeFromName(std::string const& name) {
        return name == "inOut" ? RigEase::InOut : RigEase::Linear;
    }

    template <typename T, typename ToJson>
    nlohmann::json keysToJson(std::vector<RigKey<T>> const& keys, ToJson toJson) {
        auto out = nlohmann::json::array();
        for (const auto& key : keys) {
            auto entry = nlohmann::json::array({rounded(key.time), toJson(key.value)});
            if (key.ease != RigEase::Linear) {
                entry.push_back(easeName(key.ease));
            }
            out.push_back(entry);
        }
        return out;
    }

    // Keys are [time, value] or [time, value, ease]
    template <typename T, typename FromJson>
    std::vector<RigKey<T>> keysFromJson(nlohmann::json const& json, FromJson fromJson) {
        std::vector<RigKey<T>> keys;
        for (const auto& entry : json) {
            RigKey<T> key{entry[0].get<float>(), fromJson(entry[1])};
            if (entry.size() > 2) {
                key.ease = easeFromName(entry[2].get<std::string>());
            }
            keys.push_back(key);
        }
        std::sort(keys.begin(), keys.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
        return keys;
    }
}

float RigClip::wrap(float time) const {
    if (duration <= 0.0f) {
        return 0.0f;
    }
    if (loop) {
        time = std::fmod(time, duration);
        return time < 0.0f ? time + duration : time;
    }
    return std::clamp(time, 0.0f, duration);
}

std::optional<float> RigClip::sampleRotation(std::string const& bone, float time) const {
    auto it = tracks.find(bone);
    return it == tracks.end() ? std::nullopt : sample(it->second.rotation, wrap(time), duration, loop);
}

std::optional<Vec2> RigClip::sampleOffset(std::string const& bone, float time) const {
    auto it = tracks.find(bone);
    return it == tracks.end() ? std::nullopt : sample(it->second.offset, wrap(time), duration, loop);
}

void RigClip::setRotationKey(std::string const& bone, float time, float degrees, std::optional<RigEase> ease) {
    setKey(tracks[bone].rotation, time, degrees, ease);
}

void RigClip::setOffsetKey(std::string const& bone, float time, Vec2 offset, std::optional<RigEase> ease) {
    setKey(tracks[bone].offset, time, offset, ease);
}

void RigClip::removeKeysAt(float time) {
    for (auto& [_, track] : tracks) {
        removeKeyAt(track.rotation, time);
        removeKeyAt(track.offset, time);
    }
    std::erase_if(tracks, [](auto const& entry) { return entry.second.empty(); });
}

std::vector<float> RigClip::keyTimes() const {
    std::vector<float> times;
    auto add = [&](float time) {
        for (auto existing : times) {
            if (std::abs(existing - time) < RigAnimationSet::KEY_EPSILON) return;
        }
        times.push_back(time);
    };
    for (const auto& [_, track] : tracks) {
        for (const auto& key : track.rotation) add(key.time);
        for (const auto& key : track.offset) add(key.time);
    }
    std::sort(times.begin(), times.end());
    return times;
}

void RigClip::addEvent(float time, std::string const& name) {
    events.push_back(RigClipEvent{time, name});
    std::sort(events.begin(), events.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
}

RigAnimationSet::RigAnimationSet(nlohmann::json const& json) {
    load(json);
}

RigClip* RigAnimationSet::getClip(std::string const& name) {
    auto it = clips.find(name);
    return it == clips.end() ? nullptr : &it->second;
}

void RigAnimationSet::load(nlohmann::json const& json) {
    auto vec2 = [](nlohmann::json const& j) { return Vec2(j[0].get<float>(), j[1].get<float>()); };
    auto number = [](nlohmann::json const& j) { return j.get<float>(); };

    clips.clear();

    if (!json.contains("clips")) {
        return;
    }

    for (const auto& [name, clipJson] : json["clips"].items()) {
        RigClip clip;
        clip.duration = clipJson.value("duration", 1.0f);
        clip.loop = clipJson.value("loop", true);

        if (clipJson.contains("tracks")) {
            for (const auto& [bone, trackJson] : clipJson["tracks"].items()) {
                RigBoneTrack track;
                if (trackJson.contains("rotation")) track.rotation = keysFromJson<float>(trackJson["rotation"], number);
                if (trackJson.contains("offset")) track.offset = keysFromJson<Vec2>(trackJson["offset"], vec2);
                if (!track.empty()) clip.tracks[bone] = track;
            }
        }

        if (clipJson.contains("events")) {
            for (const auto& event : clipJson["events"]) {
                clip.addEvent(event["time"].get<float>(), event["name"].get<std::string>());
            }
        }

        clips[name] = clip;
    }
}

nlohmann::json RigAnimationSet::save() const {
    auto number = [](float value) { return rounded(value); };
    auto vec2 = [](Vec2 value) { return nlohmann::json::array({rounded(value[0]), rounded(value[1])}); };

    nlohmann::json clipsJson = nlohmann::json::object();

    for (const auto& [name, clip] : clips) {
        nlohmann::json tracks = nlohmann::json::object();
        for (const auto& [bone, track] : clip.tracks) {
            nlohmann::json trackJson = nlohmann::json::object();
            if (!track.rotation.empty()) trackJson["rotation"] = keysToJson(track.rotation, number);
            if (!track.offset.empty()) trackJson["offset"] = keysToJson(track.offset, vec2);
            tracks[bone] = trackJson;
        }

        auto events = nlohmann::json::array();
        for (const auto& event : clip.events) {
            events.push_back({{"time", rounded(event.time)}, {"name", event.name}});
        }

        clipsJson[name] = {
            {"duration", rounded(clip.duration)},
            {"loop", clip.loop},
            {"tracks", tracks},
            {"events", events},
        };
    }

    return {{"clips", clipsJson}};
}

bool RigAnimationSet::saveToFile() const {
    if (filePath.empty()) {
        return false;
    }
    std::ofstream file(filePath);
    if (!file) {
        return false;
    }
    file << save().dump(4) << "\n";
    return file.good();
}

void RigAnimationSetAsset::reload() {
    m_asset = hasRigAnimationSet(m_path) ? getRigAnimationSet(m_path) : nullptr;
}
