//
// Created by henry on 2/25/24.
//

#ifndef HAGAME2_ACTION_H
#define HAGAME2_ACTION_H

#include <optional>
#include "../utils/enum.h"
#include "inputDevice.h"

namespace hg::input {

    using mapping_t = std::optional<hg::utils::enum_t>;
    using Mapping = std::tuple<mapping_t, mapping_t>;

    ENUM(MappingType)
    ENUM_VALUE(MappingType, KeyboardMouse)
    ENUM_VALUE(MappingType, Gamepad)

    template <typename Action = hg::utils::enum_t>
    struct ActionMapping {
        Action action;
        Mapping mapping;

        mapping_t keyboardMouse() const { return std::get<0>(mapping); }
        mapping_t gamepad() const { return std::get<1>(mapping); }
    };

    template <typename Action = hg::utils::enum_t>
    class ActionMapper {
    public:

        ActionMapper(std::vector<ActionMapping<Action>> buttonMap = {}, std::vector<ActionMapping<Action>> axesMap = {}) {
            for (const auto& map : buttonMap) {
                m_buttonMap.insert(std::make_pair(map.action, map.mapping));
            }
            for (const auto& map : axesMap) {
                m_axesMap.insert(std::make_pair(map.action, map.mapping));
            }
        }

        std::unordered_map<Action, bool> getKeyboardButtons(ButtonState& state) const {
            std::unordered_map<Action, bool> out;
            for (const auto& [action, mapping] : m_buttonMap) {
                auto map = std::get<0>(mapping);
                if (!map.has_value()) {
                    out.insert({action, false});
                }
                out.insert({action, state[map.value()]});
            }
            return out;
        }

        std::unordered_map<Action, bool> getGamepadButtons(ButtonState& state) const {
            std::unordered_map<Action, bool> out;
            for (const auto& [action, mapping] : m_buttonMap) {
                auto map = std::get<1>(mapping);
                if (!map.has_value()) {
                    out.insert({action, false});
                }
                out.insert({action, state[map.value()]});
            }
            return out;
        }

        std::unordered_map<Action, float> getKeyboardAxes(AxesState& state) const {
            std::unordered_map<Action, float> out;
            for (const auto& [action, mapping] : m_buttonMap) {
                auto map = std::get<0>(mapping);
                if (!map.has_value()) {
                    out.insert({action, 0});
                }
                out.insert({action, state[map.value()]});
            }
            return out;
        }

        std::unordered_map<Action, float> getGamepadAxes(AxesState& state) const {
            std::unordered_map<Action, float> out;
            for (const auto& [action, mapping] : m_buttonMap) {
                auto map = std::get<1>(mapping);
                if (!map.has_value()) {
                    out.insert({action, 0});
                }
                out.insert({action, state[map.value()]});
            }
            return out;
        }

        void clear() {
            m_buttonMap.clear();
            m_axesMap.clear();
        }

        bool hasButtonMap(Action action) const {
            return m_buttonMap.find(action) != m_buttonMap.end();
        }

        void setButtonMap(Action action, Mapping mapping) {
            m_buttonMap.insert(std::make_pair(action, mapping));
        }

        std::optional<Mapping> getButtonMap(Action action) const {
            if (!hasButtonMap(action)) {
                return std::nullopt;
            }
            return m_buttonMap.at(action);
        }

        bool hasAxesMap(Action action) const {
            return m_axesMap.find(action) != m_axesMap.end();
        }

        void setAxesMap(Action action, Mapping mapping) {
            m_axesMap.insert(std::make_pair(action, mapping));
        }

        std::optional<Mapping> getAxesMap(Action action) const {
            if (!hasAxesMap(action)) {
                return std::nullopt;
            }
            return m_axesMap.at(action);
        }

    private:

        std::unordered_map<Action, Mapping> m_buttonMap;
        std::unordered_map<Action, Mapping> m_axesMap;

    };

}

#endif //HAGAME2_ACTION_H
