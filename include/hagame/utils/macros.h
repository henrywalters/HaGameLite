//
// Created by henry on 12/22/22.
//

#ifndef HAGAME2_MACROS_H
#define HAGAME2_MACROS_H

#include "staticPool.h"

#define HG_GET(type, name) \
    type name() const               \
    {                               \
        return m_##name;            \
    }

#define HG_SET(type, name, method) \
    void name(const type& value)         \
    {                               \
        m_##name = value;           \
        method();                   \
    }

#define HG_SET_SIMPLE(type, name) \
    void name(const type& value)         \
    {                               \
        m_##name = value;           \
    }


#define HG_GET_SET(type, name, method) \
    HG_GET(type, name)                 \
    HG_SET(type, name, method)

#define HG_GET_SET_SIMPLE(type, name) \
    HG_GET(type, name)                 \
    HG_SET_SIMPLE(type, name)

#define HG_SCRIPT(name) \
    template<>                    \
    std::unordered_map<hg::utils::uuid_t, std::unique_ptr<name>> hg::utils::StaticPool<name>::s_pool = std::unordered_map<hg::utils::uuid_t, std::unique_ptr<name>>(); \
                        \
    extern "C" hg::utils::uuid_t construct_##name(hg::Scene* scene) { \
        return hg::utils::StaticPool<name>::Add(scene);                    \
    } \
                        \
    extern "C" void init_##name(hg::utils::uuid_t id) {                    \
        hg::utils::StaticPool<name>::Get(id)->init(); \
    }                   \
    \
                        \
    extern "C" void update_##name(hg::utils::uuid_t id, double dt) { \
        hg::utils::StaticPool<name>::Get(id)->update(dt);                    \
    }                   \
    \
                        \
    extern "C" void close_##name(hg::utils::uuid_t id) {                   \
        hg::utils::StaticPool<name>::Get(id)->close();                    \
    }

#define HG_COMPONENT(type) \
    extern "C" void Register_##type() { \
        hg::RegisterComponent<type>(#type); \
    }                            \

#endif //HAGAME2_MACROS_H
