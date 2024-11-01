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

#define HG_GET_PTR(type, name) \
    type* name()               \
    {                               \
        return m_##name;            \
    }

#define HG_SET(type, name, method) \
    void name(const type& value)         \
    {                               \
        m_##name = value;           \
        method();                   \
    }

#define HG_SET_PTR(type, name, method) \
    void name(type* value)         \
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

#define HG_GET_SET_PTR(type, name, method) \
    HG_GET_PTR(type, name)                 \
    HG_SET_PTR(type, name, method)

#define HG_GET_SET_SIMPLE(type, name) \
    HG_GET(type, name)                 \
    HG_SET_SIMPLE(type, name)

#define HG_SCRIPT(name) \
                        \
    template<>                    \
    std::unordered_map<hg::utils::uuid_t, std::unique_ptr<name>> hg::utils::StaticPool<name>::s_pool = std::unordered_map<hg::utils::uuid_t, std::unique_ptr<name>>(); \
                        \
    extern "C" hg::Script* new_##name(hg::Scene* scene) {                                                                                                                               \
        return new name(scene);                  \
    }                               \
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

#define HG_COMPONENT(Category, Name) static hg::ComponentFactory::attach_fn Attach_##Name = hg::ComponentFactory::Register<Name>(#Category, #Name);

#define HG_FIELD(CompName, Type, Name) static hg::ComponentFactory::ComponentField Field_##CompName##Name = hg::ComponentFactory::RegisterField( \
    #Type,                                                                                                                                    \
    #CompName,                                                                                                                               \
    #Name,                                                                                                                                    \
    [](auto component, auto value) { ((CompName*) component)->Name = std::get<Type>(value); },                                        \
    [](auto component) { return ((CompName*) component)->Name; }                                                                           \
);                                                                                                                                               \

#define HG_NUMBER_FIELD(CompName, Type, Name, Min, Max, Step) static hg::ComponentFactory::ComponentField Field_##CompName##Name = hg::ComponentFactory::RegisterField( \
    #Type,                                                                                                                                    \
    #CompName,                                                                                                                               \
    #Name,                                                                                                                                    \
    [](auto component, auto value) { ((CompName*) component)->Name = std::get<Type>(value); },                                        \
    [](auto component) { return ((CompName*) component)->Name; },                                                                                                 \
    Min,                                                                                                                                                          \
    Max,                                                                                                                                                          \
    Step                                                                                                                                                         \
);


#define HG_IS_CONCEPT(Name) template <typename T> concept Is##Name = std::is_base_of<Name, T>::value;

#define HG_SYSTEM(Category, Name) static hg::SystemFactory::RegisteredSystem System_##Name = hg::SystemFactory::Register<Name>(#Category, #Name);

#endif //HAGAME2_MACROS_H
