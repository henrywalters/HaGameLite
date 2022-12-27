//
// Created by henry on 12/22/22.
//

#ifndef HAGAME2_MACROS_H
#define HAGAME2_MACROS_H

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


#define HG_GET_SET(type, name, method) \
    HG_GET(type, name)                 \
    HG_SET(type, name, method)

#endif //HAGAME2_MACROS_H
