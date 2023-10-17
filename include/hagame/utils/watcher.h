//
// Created by henry on 7/9/23.
//

#ifndef HAGAME2_WATCHER_H
#define HAGAME2_WATCHER_H

#include <iostream>
#include <functional>

namespace hg::utils {
    template <typename T>
    class Watcher {
    public:

        Watcher() {}
        Watcher(std::function<void()> _onUpdate):
            onUpdate(_onUpdate)
        {}
        Watcher(T value): m_value(value) {}
        Watcher(T  value, std::function<void()> _onUpdate):
            m_value(value),
            onUpdate(_onUpdate)
        {}

        std::function<void()> onUpdate = [](){};

        void operator=(const T& value) {
            if (m_value != value) {
                m_value = value;
                std::cout << "NEW = " << m_value << "\n";
                onUpdate();
            }
        }

        operator T&() {
            return m_value;
        }

    private:

        T m_value;

    };
}

#endif //HAGAME2_WATCHER_H
