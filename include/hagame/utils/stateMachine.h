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

        // Instantiate a new object in the state machine
        T* add(std::string name) {
            auto state = std::make_unique<T>();
            m_states.insert(std::make_pair(name, state));
            events.emit(StateMode::Init, state.get());
            return state.get();
        }

        // Check if the state exists
        bool has(std::string name) {
            return m_states.find(name) != m_states.end();
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

            if (m_active == name) {
                events.emit(StateMode::Inactive, m_states[name].get());
                m_active = std::nullopt;
            }
        }

        void activate(std::string name) {
            if (!has(name)) {
                throw std::runtime_error("State does not exist");
            }
            if (m_active != name) {
                if (m_active.has_value()) {
                    deactivate(m_active.value());
                }

                m_active = name;
                events.emit(StateMode::Active, m_states[m_active].get());
            }
        }

        T* active() {
            if (!m_active.has_value()) {
                throw std::runtime_error("No active state");
            }
            return m_states[m_active].get();
        }

    private:

        std::unordered_map<std::string, std::unique_ptr<T>> m_states;
        std::optional<std::string> m_active;


    };
}

#endif //HAGAME2_STATEMACHINE_H
