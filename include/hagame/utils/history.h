//
// Created by henry on 9/15/26.
//

#ifndef HAGAME2_HISTORY_H
#define HAGAME2_HISTORY_H
#include <functional>

namespace hg::utils {
    class History {
    public:

        struct Action {
            std::function<void()> perform;
            std::function<void()> undo;
        };

        // Add a new action. Clears out history that happened after the current index
        void perform(Action const& action);

        // Undo N actions
        void undo(size_t n = 1);

        // Redo N actions
        void redo(size_t n = 1);

        // Clear History
        void clear();

    private:

        std::vector<Action> m_actions;
        size_t m_index = 0;

    };
}

#endif //HAGAME2_HISTORY_H
