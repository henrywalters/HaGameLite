//
// Created by henry on 1/9/23.
//
#include "../../../include/hagame/core/hg.h"

hg::Game* hg::HG::s_emscriptenGame = nullptr;

void hg::HG::Run(hg::Game *game) {

    game->initialize();

#ifdef __EMSCRIPTEN__
    s_emscriptenGame = game;
    emscripten_set_main_loop(hg::HG::TickEmscripten, 0, false);
#else

    while (game->m_window->running()) {
        game->tick();
    }

    game->m_window->close();

    glfwSwapInterval(-1);
#endif
}

void hg::HG::TickEmscripten() {
    s_emscriptenGame->tick();
}
