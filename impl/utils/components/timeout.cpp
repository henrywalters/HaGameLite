//
// Created by henry on 5/22/23.
//
#include "../../../include/hagame/utils/components/timeout.h"

hg::utils::components::Timeout::Timeout(std::function<void()> onTimeout, double duration):
    m_duration(duration),
    m_onTimeout(onTimeout)
{}

std::string hg::utils::components::Timeout::toString() const {
    return "Timeout<" + std::to_string(id()) + ">";
}

void hg::utils::components::Timeout::onUpdate(double dt) {
    m_elapsed += dt;
    if (m_elapsed >= m_duration) {
        m_onTimeout();
    }
}
