//
// Created by henry on 1/28/24.
//

#ifndef HAGAME2_ENUM_H
#define HAGAME2_ENUM_H

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include "uuid.h"

/*
 *
 * A typical enum class in c++ is quite nice, however, its lack of runtime reflection
 * cumbersome to use in game code. As a result, you often write an enum class, a vector
 * of values, and a vector of strings! The Enum class aims to simplify this pattern.
 *
 *
 * Example:
 *
 * struct A {}
 * struct B {}
 *
 *
 *
 */

namespace hg::utils {

    using enum_t = uint8_t;

    template <typename T>
    using enum_constructor_t = std::function<std::shared_ptr<T>()>;

    template <typename T>
    using enum_factory_t = std::unordered_map<enum_t, enum_constructor_t<T>>;

    struct EnumElement {
        enum_t key;
        std::string label;

        EnumElement(std::string _label):
            label(_label)
        {}

        bool operator==(const EnumElement& el) const {
            return key == el.key;
        }

        bool operator!=(const EnumElement& el) const {
            return key != el.key;
        }

        bool operator >(const EnumElement& el) const {
            return key > el.key;
        }
    };

    std::vector<EnumElement>* GetEnum(hg::utils::uuid_t id);

    template <typename T>
    enum_factory_t<T>* GetEnumFactory(hg::utils::uuid_t id) {
        static std::unordered_map<hg::utils::uuid_t, hg::utils::enum_factory_t<T>> EnumFactory;
        if (EnumFactory.find(id) == EnumFactory.end()) {
            EnumFactory.insert(std::make_pair(id, hg::utils::enum_factory_t<T>()));
        }
        return &EnumFactory.at(id);
    }

    // Register an enum value. Offsets allow multiple enums to non-exclusively use one enum_t
    enum_t RegisterValue(hg::utils::uuid_t enumId, std::string label, int offset = 0);

    template <typename Base, typename Ext>
    enum_constructor_t<Ext> RegisterFactoryMethod(hg::utils::uuid_t factoryId, enum_t value) {
        auto factory = GetEnumFactory<Base>(factoryId);
        auto fn = [](){ return std::make_shared<Ext>(); };
        factory->insert(std::make_pair(value, fn));
        return fn;
    }

}

#define ENUM(Enum) namespace Enum { \
    const hg::utils::uuid_t _ID = hg::utils::UUID::Generate(); \
}                                   \

#define ENUMS(Enum) hg::utils::GetEnum(Enum::_ID)

#define ENUM_VALUE(Enum, Value) namespace Enum { \
    const hg::utils::enum_t Value = hg::utils::RegisterValue(_ID, #Value);                             \
}

#define ENUM_VALUE_OFFSET(Enum, Value, Offset) namespace Enum { \
    const hg::utils::enum_t Value = hg::utils::RegisterValue(_ID, #Value, Offset);                             \
}

#define ENUM_FACTORY(Enum) namespace Enum { \
    const hg::utils::uuid_t _FACTORY_ID = hg::utils::UUID::Generate(); \
}                                           \

#define FACTORY_VALUE(Enum, Value, Base, Ext) namespace Enum { \
    const hg::utils::enum_constructor_t<Ext> Make_##Value = hg::utils::RegisterFactoryMethod<Base, Ext>(_FACTORY_ID, Enum::Value); \
}                                                              \

#define GET_FACTORY(Enum, Base) hg::utils::GetEnumFactory<Base>(Enum::_FACTORY_ID)

#endif //HAGAME2_ENUM_H
