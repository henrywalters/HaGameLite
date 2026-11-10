//
// Created by henry on 10/16/26.
//
#include "../../include/hagame/graphics/rigAtlas.h"
#include "../../include/hagame/core/assets.h"

using namespace hg;
using namespace hg::graphics;

RigAtlas::RigAtlas(nlohmann::json const& json) {
    load(json);
}

Texture* RigAtlas::texture() const {
    return getTexture(texturePath);
}

const RigPart* RigAtlas::getPart(std::string const& name) const {
    auto it = m_partIndex.find(name);
    return it == m_partIndex.end() ? nullptr : &parts[it->second];
}

std::optional<Vec2> RigAtlas::getJoint(std::string const& name) const {
    auto it = joints.find(name);
    if (it == joints.end()) {
        return std::nullopt;
    }
    return it->second;
}

void RigAtlas::load(nlohmann::json const& json) {
    auto vec2 = [](nlohmann::json const& j) { return Vec2(j[0].get<float>(), j[1].get<float>()); };
    auto vec2i = [](nlohmann::json const& j) { return Vec2i(j[0].get<int>(), j[1].get<int>()); };

    texturePath = json["texture"].get<std::string>();
    textureSize = vec2i(json["textureSize"]);
    canvasSize = vec2i(json["canvasSize"]);

    parts.clear();
    m_partIndex.clear();
    for (const auto& part : json["parts"]) {
        auto uv = part["uv"];
        RigPart rigPart{
            part["name"].get<std::string>(),
            Rect(Vec2(uv[0].get<float>(), uv[1].get<float>()), Vec2(uv[2].get<float>(), uv[3].get<float>())),
            vec2(part["size"]),
            vec2(part["origin"]),
        };
        m_partIndex[rigPart.name] = parts.size();
        parts.push_back(rigPart);
    }

    joints.clear();
    for (const auto& [name, point] : json["joints"].items()) {
        joints[name] = vec2(point);
    }
}

void RigAtlasAsset::reload() {
    m_asset = hasRigAtlas(m_path) ? getRigAtlas(m_path) : nullptr;
}
