//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_GLFW_H
#define HAGAME2_GLFW_H

// Include the Emscripten library only if targetting WebAssembly
#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
    #define GLFW_INCLUDE_ES3
    #include "../../../thirdparty/glad/glad.h"
#else
    #include "../../../thirdparty/glad/glad.h"
#endif

#include <GLFW/glfw3.h>

#endif //HAGAME2_GLFW_H
