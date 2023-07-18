//
// Created by henry on 7/9/23.
//

#ifndef HAGAME2_WATCHER_H
#define HAGAME2_WATCHER_H

#include <functional>

namespace hg::utils {
    template <typename T>
    class Watcher {
    public:

        Watcher() {}
        Watcher(T value): m_value(value) {}

        std::function<void()> onUpdate = [](){};

        void operator=(const T& value) {
            if (m_value != value) {
                m_value = value;
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
