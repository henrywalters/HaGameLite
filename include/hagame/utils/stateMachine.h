//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_STATEMACHINE_H
#define HAGAME2_STATEMACHINE_H

#include <unordered_map>
#include <memory>
#include <optional>

#include "pubsub.h"

namespace hg::utils {

    enum class StateMode {
        Init,
        Active,
        Inactive,
    };

    template <class T>
    class StateMachine {
    public:

        Publisher<StateMode, T*> events;

        StateMachine():
            m_active(std::nullopt)
        {}

        void clear() {
            m_states.clear();
        }

        // Instantiate a new object in the state machine
        template <class SceneType>
        T* add(std::string name) {
            m_states.insert(std::make_pair(name, std::make_unique<SceneType>()));
            events.emit(StateMode::Init, m_states[name].get());
            return m_states[name].get();
        }

        // Check if the state exists
        bool has(std::string name) {
            return m_states.find(name) != m_states.end();
        }

        bool hasActive() {
            return m_active.has_value();
        }

        // Activate a state without triggering an event
        void setActive(std::string name) {
            if (!has(name)) {
                throw std::runtime_error("State does not exist");
            }
            m_active = name;
        }

        void deactivate(std::string name) {
            if (!has(name)) {
                throw std::runtime_error("State does not exist");
            }

            if (m_active.value() == name) {
                events.emit(StateMode::Inactive, m_states[name].get());
                m_active = std::nullopt;
            }
        }

        void activate(std::string name) {
            if (!has(name)) {
                throw std::runtime_error("State does not exist");
            }
            if (m_active.has_value() && m_active.value() != name) {
                deactivate(m_active.value());
            }

            m_active = name;
            events.emit(StateMode::Active, m_states[m_active.value()].get());
        }

        T* active() {
            if (!hasActive()) {
                throw std::runtime_error("No active state");
            }
            return m_states[m_active.value()].get();
        }

    private:

        std::unordered_map<std::string, std::unique_ptr<T>> m_states;
        std::optional<std::string> m_active;


    };
}

#endif //HAGAME2_STATEMACHINE_H
