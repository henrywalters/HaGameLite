//
// Created by henry on 12/22/22.
//

#ifndef HAGAME2_MACROS_H
#define HAGAME2_MACROS_H

#define HG_GET_SET(type, name, method) \
    void name(const type& value)         \
    {                               \
        m_##name = value;           \
        method();                   \
    }                               \
                                    \
    type name() const               \
    {                               \
        return m_##name;            \
    }                               \

#endif //HAGAME2_MACROS_H
