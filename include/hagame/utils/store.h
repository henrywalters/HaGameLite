//
// Created by henry on 1/11/23.
//

#ifndef HAGAME2_STORE_H
#define HAGAME2_STORE_H

#include "pubsub.h"

namespace hg::utils {

    enum class StoreEvent {
        Init,
        Set,
        Get,
    };

    template <typename Key, typename Value>
    class Store {
    public:

        Publisher<StoreEvent, std::tuple<Key, Value>> events;

        Store() {}

        Store(std::vector<std::tuple<Key, Value>> values) {
            for (const auto& [key, value] : values) {
                set(key, value);
            }
        }

        void set(Key key, Value value) {
            if (!has(key)) {
                m_store.insert(std::make_pair(key, value));
                events.emit(StoreEvent::Init, std::make_tuple(key, value));
            }

            m_store[key] = value;
            events.emit(StoreEvent::Set, std::make_tuple(key, value));
        }

        bool has(Key key) const {
            return m_store.find(key) != m_store.end();
        }

        Value get(Key key) const {
            if (!has(key)) {
                throw new std::runtime_error("Key does not exist in store");
            }
            // events.emit(StoreEvent::Get, std::make_tuple(key, get(key)));
            return m_store.at(key);
        }

        void forEach(std::function<void(Key, Value)> fn) const {
            for (const auto& [key, value] : m_store) {
                fn(key, value);
            }
        }

        std::unordered_map<Key, Value>& store() {
            return m_store;
        }

        void clear() {
            m_store.clear();
        }

    private:

        std::unordered_map<Key, Value> m_store;

    };
}

#endif //HAGAME2_STORE_H
