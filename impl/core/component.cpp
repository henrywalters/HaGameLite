//
// Created by henry on 2/17/23.
//
#include "../../../include/hagame/core/component.h"

entt::meta_type hg::GetComponent(std::string id) {
    return entt::resolve(entt::hashed_string{id.c_str()}.value());
}