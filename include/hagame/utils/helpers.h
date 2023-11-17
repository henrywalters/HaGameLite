//
// Created by henry on 10/10/23.
//

#ifndef HAGAME2_HELPERS_H
#define HAGAME2_HELPERS_H

#include <functional>
#include <vector>

namespace hg::utils {
    template <typename T>
    inline void v_remove(std::vector<T> arr, const T& el) {
        auto pos = std::find(arr.begin(), arr.end(), el);
        if (pos != arr.end()) {
            arr.erase(pos);
        }
    }

    template <typename T, typename V>
    inline bool contains(const T& arr, const V& el) {
        return std::find(arr.begin(), arr.end(), el) != arr.end();
    }

    template <typename T, typename V>
    inline bool contains(const std::unordered_map<T, V>& map, const T& key) {
        return map.find(key) != map.end();
    }
}



#endif //HAGAME2_HELPERS_H
