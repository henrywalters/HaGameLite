//
// Created by henry on 10/22/23.
//

#ifndef HAGAME2_VARIANT_H
#define HAGAME2_VARIANT_H

#include <variant>
#include <string>
#include <vector>
#include "../math/aliases.h"
#include "../core/entity.h"
#include "string.h"

namespace hg::utils {

    // The hg::utils::variant provides a variant type with additional helper utilities for types specific to HaGame
    using variant = std::variant<
            bool,
            float,
            int,
            double,
            std::string,
            Vec3,
            Vec2,
            Vec3i,
            Vec2i,
            GameObject*,
            Entity*,
            std::vector<float>,
            std::vector<bool>,
            std::vector<int>,
            std::vector<double>,
            std::vector<std::string>,
            std::vector<Vec3>,
            std::vector<Vec2>,
            std::vector<Vec3i>,
            std::vector<Vec2i>,
            std::vector<Entity*>,
            std::vector<GameObject*>
    >;

    template <size_t size, class T>
    inline std::string hg_vector_to_string(const std::vector<math::Vector<size, T>>& values) {
        std::vector<std::string> strValues;
        for (const auto& value : values) {
            strValues.push_back(value.toString());
        }
        return "{" + s_join(strValues, ", ") + "}";
    }

    template <typename T>
    inline std::string vector_to_string(const std::vector<T>& values) {
        std::vector<std::string> strValues;
        for (const auto& value : values) {
            strValues.push_back(std::to_string(value));
        }
        return "{" + s_join(strValues, ", ") + "}";
    }

    inline std::string vector_to_string(const std::vector<std::string>& values) {
        return "{" + s_join(values, ", ") + "}";
    }

    inline std::string vector_to_string(const std::vector<Entity*>& values) {
        std::vector<std::string> strValues;
        for (const auto& value : values) {
            strValues.push_back(value->name);
        }
        return "{" + s_join(strValues, ", ") + "}";
    }

    inline std::string vector_to_string(const std::vector<GameObject*>& values) {
        std::vector<std::string> strValues;
        for (const auto& value : values) {
            strValues.push_back(std::to_string(value->id()));
        }
        return "{" + s_join(strValues, ", ") + "}";
    }

#define SIMPLE_VALUE(type) std::string operator()(type value) { return std::to_string(value); }
#define OBJ_VALUE(type) std::string operator()(type value) { return value.toString(); }
#define VECTOR_VALUE(type) std::string operator()(std::vector<type> values) { return vector_to_string(values); }
#define HG_VECTOR_VALUE(type) std::string operator()(std::vector<type> values) { return hg_vector_to_string(values); }

    struct DataContextPrint {
        std::string operator()(bool value) { return value ? "true" : "false"; }
        SIMPLE_VALUE(float)
        SIMPLE_VALUE(int)
        SIMPLE_VALUE(double)
        std::string operator()(std::string value) { return value; }
        OBJ_VALUE(Vec3)
        OBJ_VALUE(Vec2)
        OBJ_VALUE(Vec3i)
        OBJ_VALUE(Vec2i)
        std::string operator()(Entity* entity) { return entity->name; }
        std::string operator()(GameObject* obj) { return std::to_string(obj->id()); }
        VECTOR_VALUE(float)
        VECTOR_VALUE(bool)
        VECTOR_VALUE(int)
        VECTOR_VALUE(double)
        VECTOR_VALUE(std::string)
        HG_VECTOR_VALUE(Vec3)
        HG_VECTOR_VALUE(Vec2)
        HG_VECTOR_VALUE(Vec3i)
        HG_VECTOR_VALUE(Vec2i)
        VECTOR_VALUE(Entity*)
        VECTOR_VALUE(GameObject*)
    };

    inline std::string toString(const variant& var) {
        return std::visit(DataContextPrint{}, var);
    }

}

#endif //HAGAME2_VARIANT_H
