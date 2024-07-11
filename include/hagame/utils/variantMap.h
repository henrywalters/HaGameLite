//
// Created by henry on 7/6/24.
//

#ifndef HAGAME2_VARIANTMAP_H
#define HAGAME2_VARIANTMAP_H

#include "watcher.h"
#include "variant.h"

namespace hg::utils {
    struct VariantMap {
        std::unordered_map<std::string, hg::utils::Watcher<variant>> map;

        template <typename T>
        hg::utils::Watcher<T> registerField(std::string field, T defaultValue) {
            map.insert(std::make_pair(field, defaultValue));
            Watcher<T> value(defaultValue, [&]() {
                map.at(field) = value.value();
            });
            return value;
        }
    };
}

#define V_FIELD(Type, Name, DefaultValue) hg::utils::Watcher<Type> Name = ([&](){ \
    hg::utils::Watcher<Type> rawValue(DefaultValue, [&]() {                                  \
        std::cout << "SET V FIELD FROM watcher\n";                                                                          \
        map.at(#Name) = rawValue.value();                                                                              \
    });                                                                                  \
                                                                                  \
    hg::utils::Watcher<Type> mapValue(DefaultValue, [&]() {                                  \
        std::cout << "SET V FIELD FROM map\n";                                                                          \
        rawValue = mapValue.value();                                                                              \
    });                                                                                  \
                                                                                  \
    map.insert(std::make_pair(#Name, mapValue));                                                                              \
                                                                                  \
    return rawValue;                                                                                  \
})();

#endif //HAGAME2_VARIANTMAP_H
