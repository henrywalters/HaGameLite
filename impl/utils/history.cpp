//
// Created by henry on 9/15/26.
//
#include <hagame/utils/history.h>

using namespace hg::utils;

void History::perform(Action const &action) {
    if (m_index > m_actions.size()) {
        m_actions.resize(m_index);
    }
    m_actions.push_back(action);
    action.perform();
    m_index++;
}

void History::undo(size_t n) {
    for (size_t i = m_index; i < m_index - n; i--) {
        if (i >= 0) {
            m_actions[i].undo();
            m_index--;
        }
    }
}

void History::redo(size_t n) {
    for (size_t i = m_index; i < m_index + n; i++) {
        if (i < m_actions.size()) {
            m_actions[i].perform();
            m_index++;
        }
    }
}

void History::clear() {
    m_actions.clear();
    m_index = 0;
}
