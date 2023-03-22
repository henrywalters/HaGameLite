//
// Created by henry on 3/3/23.
//

#ifndef HAGAME2_STATICPOOL_H
#define HAGAME2_STATICPOOL_H

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include "uuid.h"

namespace hg::utils {

    template <class T>
    class StaticPool {
    public:

        template <class ...Args>
        static uuid_t Add(Args&&... args) {
            UUID id;
            s_pool.insert(std::make_pair(id.id(), std::make_unique<T>(std::forward<Args>(args)...)));
            return id.id();
        }

        static T* Get(uuid_t id) {
            if (s_pool.find(id) == s_pool.end()) {
                throw std::runtime_error(("Static Instance: " + std::to_string(id) + " does not exist.").c_str());
            }
            return s_pool[id].get();
        }

        static void Clear() {
            s_pool.clear();
        }

        static std::unordered_map<uuid_t, std::unique_ptr<T>> s_pool;
    };

}

#endif //HAGAME2_STATICPOOL_H
