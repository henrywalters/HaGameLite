//
// Created by henry on 1/9/23.
//
#include "../../../include/hagame/core/hg.h"

hg::Game* hg::HG::s_emscriptenGame = nullptr;

void hg::HG::Run(hg::Game *game) {

    Start(game);

#ifdef __EMSCRIPTEN__
    s_emscriptenGame = game;
    emscripten_set_main_loop(hg::HG::TickEmscripten, 0, false);
#else

    while (game->running()) {
        game->tick();
    }

    End(game);

#endif
}

void hg::HG::RunAsync(hg::Game *game) {
    game->initialize();
    game->run();
}

void hg::HG::TickEmscripten() {
    s_emscriptenGame->tick();
}

void hg::HG::Start(hg::Game *game) {
    glfwInit();
    game->initialize();
}

void hg::HG::End(hg::Game *game) {
    game->destroy();
}

