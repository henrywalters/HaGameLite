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
#include "uuid.h"
#include "hagame/graphics/texture.h"
#include "../ui/style.h"

namespace hg::utils {

    // The hg::utils::variant provides a variant type with additional helper utilities for types specific to HaGame
    using variant = std::variant<
        void*,
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
        std::vector<void*>,
        hg::Rect,
        UUID,
        uuid_t,
        hg::graphics::TextureAsset
    >;

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

#define D_HG_VECTOR(Type, DataType, Size) if (type == #Type) { \
        auto parts = hg::utils::s_split(raw, ',');             \
        Type out;                                              \
        for (int i = 0; i < Size; i++) {                             \
            out.vector[i] = strToT<DataType>(parts[i]);                  \
        }                                                                \
        return out;                                                      \
    } \

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
        S_SIMPLE_VECTOR(void*)
        S_SIMPLE_VECTOR(std::string)
        S_LIST_OF_HG_VECTORS(hg::Vec4, float, 4)
        S_LIST_OF_HG_VECTORS(hg::Vec3, float, 3)
        S_LIST_OF_HG_VECTORS(hg::Vec2, float, 2)
        S_LIST_OF_HG_VECTORS(hg::Vec4i, int, 4)
        S_LIST_OF_HG_VECTORS(hg::Vec3i, int, 3)
        S_LIST_OF_HG_VECTORS(hg::Vec2i, int, 2)
        std::string operator()(hg::Rect value) { return vector_to_string(std::vector<float>{value.pos[0], value.pos[1], value.size[0], value.size[1]}); }
        std::string operator()(hg::graphics::TextureAsset value) { return value.path(); }
    };

    inline std::string serialize(const variant& var) {
        return std::visit( PrintVariant{}, var);
    }

    inline variant deserialize(std::string type, std::string raw) {
        if (type == "hg::utils::UUID") {
            return UUID(strToT<long>(raw));
        }
        D_SIMPLE(bool)
        D_SIMPLE(float)
        D_SIMPLE(int)
        D_SIMPLE(double)
        D_SIMPLE(void*)
        D_SIMPLE(hg::utils::uuid_t)
        if (type == "std::string") { return raw; }
        D_HG_VECTOR(hg::graphics::Color, float, 4)
        D_HG_VECTOR(hg::Vec4, float, 4)
        D_HG_VECTOR(hg::Vec3, float, 3)
        D_HG_VECTOR(hg::Vec2, float, 2)
        D_HG_VECTOR(hg::Vec4i, int, 4)
        D_HG_VECTOR(hg::Vec3i, int, 3)
        D_HG_VECTOR(hg::Vec2i, int, 2)
        if (type == "hg::graphics::TextureAsset") {
            return hg::graphics::TextureAsset(raw);
        }
        if (type == "hg::Rect") {
            auto parts = s_split(raw, ',');
            return hg::Rect(Vec2(std::stof(parts[0]), std::stof(parts[1])), Vec2(std::stof(parts[2]), std::stof(parts[3])));
        }

        throw std::runtime_error("Unsupported variant type parser: " + type);
    }
}

#endif //HAGAME2_VARIANT_H
