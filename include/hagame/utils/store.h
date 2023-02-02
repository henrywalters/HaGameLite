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

    template <class T>
    class Store {
    public:

        Publisher<StoreEvent, std::tuple<std::string, T>> events;

        void set(std::string key, T value) {
            if (!has(key)) {
                m_store.insert(std::make_pair(key, value));
                events.emit(StoreEvent::Init, std::make_tuple(key, value));
            }

            m_store[key] = value;
            events.emit(StoreEvent::Set, std::make_tuple(key, value));
        }

        bool has(std::string key) {
            return m_store.find(key) != m_store.end();
        }

        T get(std::string key) {
            if (!has(key)) {
                throw new std::runtime_error("Key does not exist in store");
            }
            events.emit(StoreEvent::Get, std::make_tuple(key, m_store[key]));
            return m_store[key];
        }

    private:

        std::unordered_map<std::string, T> m_store;

    };
}

#endif //HAGAME2_STORE_H
