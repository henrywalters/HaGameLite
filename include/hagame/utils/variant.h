//
// Created by henry on 10/22/23.
//

#ifndef HAGAME2_VARIANT_H
#define HAGAME2_VARIANT_H

#include <variant>
#include <string>
#include <vector>
#include "../math/aliases.h"
#include "string.h"
#include "../graphics/color.h"
#include "config.h"
#include "spatialMap.h"
#include "uuid.h"
#include "hagame/graphics/texture.h"
#include "../ui/style.h"
#include "hagame/core/prefabAsset.h"
#include "hagame/graphics/spriteSheetV2.h"

namespace hg::utils {
    constexpr bool is_string_type(std::string const& type) {
        return type == "std::string" || type == "hg::graphics::tilemap_path_t";
    }

    // The hg::utils::variant provides a variant type with additional helper utilities for types specific to HaGame
    using variant = std::variant<
        //void*,
        bool,
        float,
        int,
        double,
        std::string,
        hg::graphics::Color,
        Vec4,
        Vec3,
        Vec2,
        Vec4i,
        Vec3i,
        Vec2i,
        std::vector<float>,
        std::vector<bool>,
        std::vector<int>,
        std::vector<double>,
        std::vector<std::string>,
        std::vector<Vec4i>,
        std::vector<Vec3>,
        std::vector<Vec2>,
        std::vector<Vec3i>,
        std::vector<Vec2i>,
        hg::Rect,
        UUID,
        uuid_t,
        hg::graphics::TextureAsset,
        hg::graphics::SpriteSheetAsset,
        hg::PrefabAsset
        // SpatialMap2D<hg::graphics::Color, int>
    >;

#define VECTOR_TO_JSON(Type, DataType, Size) inline void to_json(nlohmann::json& j, const Type& v) { j = v.vector; } \
    inline void from_json(const nlohmann::json& j, Type& v) {v.vector = std::get<std::array<DataType, Size>(j); }

    template <size_t size, class T>
    inline std::string hg_vector_to_string(std::vector<math::Vector<size, T>> values) {
        std::string out;
        for (auto& value : values) {
            std::string serialized = arrToString<T, size>(value.vector);
            out += "{" + serialized + "}";
        }
        return out;
    }

    template <typename T>
    inline std::string vector_to_string(const std::vector<T>& values) {
        std::vector<std::string> strValues;
        for (const auto& value : values) {
            strValues.push_back(std::to_string(value));
        }
        return s_join(strValues, ",");
    }

    inline std::string vector_to_string(const std::vector<std::string>& values) {
        return s_join(values, ",");
    }

    inline std::string vector_to_string(const std::vector<void*>& values) {
        std::vector<std::string> strValues;
        for (const auto& value : values) {
            strValues.push_back("");
        }
        return s_join(strValues, ",");
    }

    template <size_t N, typename T>
    inline void string_to_vector(const std::string& str, std::array<T, N>& vector) {
        auto parts = s_split(str, ',');
        for (int i = 0; i < parts.size(); i++) {
            vector[i] = strToT<T>(parts[i]);
        }
    }

#define SIMPLE_VALUE(type) std::string operator()(type value) { return std::to_string(value); }
#define OBJ_VALUE(type) std::string operator()(type value) { return value.toString(); }
#define VECTOR_VALUE(type) std::string operator()(std::vector<type> values) { return vector_to_string(values); }
#define HG_VECTOR_VALUE(type) std::string operator()(std::vector<type> values) { return hg_vector_to_string(values); }

#define S_SIMPLE(Type) std::string operator()(Type value) { return std::to_string(value); }
#define S_HG_VECTOR(Type, DataType, Size) std::string operator()(Type value) { return hg::utils::arrToString<DataType, Size>(value.vector); }
#define S_SIMPLE_VECTOR(Type) std::string operator()(std::vector<Type> values) { return vector_to_string(values); }
#define S_LIST_OF_HG_VECTORS(Type, DataType, Size) std::string operator()(std::vector<Type>(values)) { return hg_vector_to_string(values); }

#define D_SIMPLE(Type) if (type == #Type) { \
return strToT<Type>(raw);                     \
}                                                 \

#define D_HG_VECTOR(Type, DataType, Size) if (type == #Type) {\
    Type vector; \
    string_to_vector<Size, DataType>(raw, vector.vector); \
    return vector;\
}

    struct PrintVariant {
        S_SIMPLE(bool)
        S_SIMPLE(int)
        S_SIMPLE(float)
        S_SIMPLE(double)
        std::string operator()(std::string value) { return value; }
        std::string operator()(UUID uuid) { return std::to_string(uuid.id()); }
        S_SIMPLE(uuid_t)
        S_HG_VECTOR(hg::graphics::Color, float, 4)
        S_HG_VECTOR(hg::Vec4, float, 4)
        S_HG_VECTOR(hg::Vec3, float, 3)
        S_HG_VECTOR(hg::Vec2, float, 2)
        S_HG_VECTOR(hg::Vec4i, int, 4)
        S_HG_VECTOR(hg::Vec3i, int, 3)
        S_HG_VECTOR(hg::Vec2i, int, 2)
        S_SIMPLE_VECTOR(bool)
        S_SIMPLE_VECTOR(int)
        S_SIMPLE_VECTOR(float)
        S_SIMPLE_VECTOR(double)
        //S_SIMPLE_VECTOR(void*)
        S_SIMPLE_VECTOR(std::string)
        S_LIST_OF_HG_VECTORS(hg::Vec4, float, 4)
        S_LIST_OF_HG_VECTORS(hg::Vec3, float, 3)
        S_LIST_OF_HG_VECTORS(hg::Vec2, float, 2)
        S_LIST_OF_HG_VECTORS(hg::Vec4i, int, 4)
        S_LIST_OF_HG_VECTORS(hg::Vec3i, int, 3)
        S_LIST_OF_HG_VECTORS(hg::Vec2i, int, 2)
        std::string operator()(hg::Rect value) { return vector_to_string(std::vector<float>{value.pos[0], value.pos[1], value.size[0], value.size[1]}); }
        std::string operator()(hg::graphics::TextureAsset value) { return value.path(); }
        std::string operator()(hg::graphics::SpriteSheetAsset value) { return value.path(); }
        std::string operator()(hg::PrefabAsset value) { return value.path(); }
        std::string operator()(hg::utils::SpatialMap2D<hg::graphics::Color, int> value) {
            std::vector<std::string> parts;
            value.forEach([&parts](Vec2i idx, graphics::Color color) {
                parts.push_back(utils::arrToString<int, 2>(idx.vector) + ":" + utils::arrToString<float, 4>(color.vector));
            });
            return s_join(parts, "|");
        }
    };

    inline std::string serialize(const variant& var) {
        return std::visit( PrintVariant{}, var);
    }

    inline variant deserialize(std::string const& type, std::string const& raw) {
        if (type == "hg::utils::UUID") {
            return UUID(strToT<long>(raw));
        }
        D_SIMPLE(bool)
        D_SIMPLE(float)
        D_SIMPLE(int)
        D_SIMPLE(double)
        //D_SIMPLE(void*)
        D_SIMPLE(hg::utils::uuid_t)
        if (is_string_type(type)) { return raw; }
        D_HG_VECTOR(hg::graphics::Color, float, 4)
        D_HG_VECTOR(hg::Vec4, float, 4)
        D_HG_VECTOR(hg::Vec3, float, 3)
        D_HG_VECTOR(hg::Vec2, float, 2)
        D_HG_VECTOR(hg::Vec4i, int, 4)
        D_HG_VECTOR(hg::Vec3i, int, 3)
        D_HG_VECTOR(hg::Vec2i, int, 2)
        if (type == "hg::graphics::TextureAsset") {
            hg::graphics::TextureAsset asset;
            asset.path(raw);
            return asset;
        }
        if (type == "hg::graphics::SpriteSheetAsset") {
            hg::graphics::SpriteSheetAsset asset;
            asset.path(raw);
            return asset;
        }
        if (type == "hg::PrefabAsset") {
            hg::PrefabAsset asset;
            asset.path(raw);
            return asset;
        }
        if (type == "hg::Rect") {
            auto parts = s_split(raw, ',');
            return hg::Rect(Vec2(std::stof(parts[0]), std::stof(parts[1])), Vec2(std::stof(parts[2]), std::stof(parts[3])));
        }

        // if (type == "color_tilemap_t") {
        //     auto parts = s_split(raw, '|');
        //     SpatialMap2D<hg::graphics::Color, int> map;
        //
        //     for (const auto& part : parts) {
        //         if (part.empty()) continue;
        //         auto subParts = s_split(part, ':');
        //         hg::Vec2i idx;
        //         string_to_vector(subParts[0], idx.vector);
        //         graphics::Color color;
        //         string_to_vector(subParts[1], color.vector);
        //         map.set(idx, color);
        //     }
        //
        //     return map;
        // }

        throw std::runtime_error("Unsupported variant type parser: " + type);
    }
}

#endif //HAGAME2_VARIANT_H
