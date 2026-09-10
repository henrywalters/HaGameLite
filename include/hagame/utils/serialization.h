//
// Created by henry on 7/29/26.
//

#ifndef HAGAME2_SERIALIZATION_H
#define HAGAME2_SERIALIZATION_H

#include <nlohmann/json.hpp>
#include "variant.h"
#include "hagame/graphics/spriteSheetV2.h"

namespace hg {
    inline void to_json(nlohmann::json& j, PrefabAsset const& prefab) {
        j = prefab.path();
    }

    inline void from_json(nlohmann::json const& j, PrefabAsset& prefab) {
        prefab.path(j.get<std::string>());
    }
}

namespace hg::graphics {
    inline void to_json(nlohmann::json& j, TextureAsset const& texture) {
        j = texture.path();
    }

    inline void from_json(nlohmann::json const& j, TextureAsset& texture) {
        texture.path(j.get<std::string>());
    }

    inline void to_json(nlohmann::json& j, SpriteSheetAsset const& sheet) {
        j = sheet.path();
    }

    inline void from_json(nlohmann::json const& j, SpriteSheetAsset& sheet) {
        sheet.path(j.get<std::string>());
    }

    inline void to_json(nlohmann::json& j, Color const& color) {
        j = color.vector;
    }

    inline void from_json(nlohmann::json const& j, Color& color) {
        color.vector = j.get<std::array<float, 4>>();
    }
}

namespace hg::math {

    template <size_t Size, typename T>
    inline void to_json(nlohmann::json& j, Vector<Size, T> const& v) {
        j = v.vector;
    }

    template <size_t Size, typename T>
    inline void from_json(nlohmann::json const& j, Vector<Size, T>& v) {
        v.vector = j.get<std::array<T, Size>>();
    }

    inline void to_json(nlohmann::json& j, Quat const& quat) {
        j = quat.vector;
    }

    inline void from_json(nlohmann::json const& j, Quat& quat) {
        quat.vector = j.get<std::array<float, 4>>();
    }

    template <size_t dimensions, typename T>
    inline void to_json(nlohmann::json& j, const Hypercube<dimensions, T>& h) {
        j = nlohmann::json{
                    {"pos", h.pos},
                    {"size", h.size}
        };
    }

    template <size_t dimensions, typename T>
    inline void from_json(const nlohmann::json& j, Hypercube<dimensions, T>& h) {
        j.at("pos").get_to(h.pos);
        j.at("size").get_to(h.size);
    }

}

namespace hg::utils {

    inline void to_json(nlohmann::json& j, UUID const& uuid) {
        j = uuid.id();
    }

    inline void from_json(nlohmann::json const& j, UUID& uuid) {
        uuid.setId(j.get<uuid_t>());
    }


    inline nlohmann::json serializeToJson(variant const& var) {
        nlohmann::json j;
        std::visit([&j](auto&& value) { j = value;}, var);
        return j;
    }

    template <typename T>
    inline variant as(nlohmann::json const& j) {
        return j.get<T>();
    }

    inline variant deserializeFromJson(std::string const& type, nlohmann::json const& j) {
        static const std::unordered_map<std::string, std::function<variant(nlohmann::json const&)>> parsers = {
            {"bool", as<bool>},
            {"float", as<float>},
            {"int", as<int>},
            {"double", as<double>},
            {"std::string", as<std::string>},
            {"hg::graphics::Color", as<hg::graphics::Color>},
            {"hg::Vec4", as<Vec4>},
            {"hg::Vec3", as<Vec3>},
            {"hg::Vec2", as<Vec2>},
            {"hg::Vec4i", as<Vec4i>},
            {"hg::Vec3i", as<Vec3i>},
            {"hg::Vec2i", as<Vec2i>},
            {"hg::Quat", as<hg::Quat>},
            {"std::vector<float>", as<std::vector<float>>},
            {"std::vector<bool>", as<std::vector<bool>>},
            {"std::vector<int>", as<std::vector<int>>},
            {"std::vector<double>", as<std::vector<double>>},
            {"std::vector<std::string>", as<std::vector<std::string>>},
            {"std::vector<hg::Vec4i>", as<std::vector<Vec4i>>},
            {"std::vector<hg::Vec3>", as<std::vector<Vec3>>},
            {"std::vector<hg::Vec2>", as<std::vector<Vec2>>},
            {"std::vector<hg::Vec3i>", as<std::vector<Vec3i>>},
            {"std::vector<hg::Vec2i>", as<std::vector<Vec2i>>},
            {"hg::Rect", as<hg::Rect>},
            {"hg::utils::UUID", as<UUID>},
            {"uuid_t", as<uuid_t>},
            {"hg::graphics::TextureAsset", as<hg::graphics::TextureAsset>},
            {"hg::graphics::SpriteSheetAsset", as<hg::graphics::SpriteSheetAsset>},
            {"hg::PrefabAsset", as<hg::PrefabAsset>},
        };

        auto it = parsers.find(type);
        if (it == parsers.end()) throw std::runtime_error("Unsupported variant type" + type + " in deserializer");
        return it->second(j);
    }

}

#endif //HAGAME2_SERIALIZATION_H
