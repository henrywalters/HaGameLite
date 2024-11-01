//
// Created by henry on 10/29/24.
//

#ifndef HAGAME2_UTILS_ALIASES_H
#define HAGAME2_UTILS_ALIASES_H

#include <vector>
#include <array>
#include <unordered_map>
#include <set>
#include <functional>

namespace hg {

    template <typename T>
    using List = std::vector<T>;

    template <typename T, size_t N>
    using Array = std::array<T, N>;

    template <typename K, typename V>
    using Map = std::unordered_map<K, V>;

    using VoidFunc = std::function<void()>;

}

#endif //HAGAME2_ALIASES_H
