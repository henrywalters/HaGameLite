//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_GLFW_H
#define HAGAME2_GLFW_H

// Include the Emscripten library only if targetting WebAssembly
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#endif

#include <GLFW/glfw3.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>

#endif //HAGAME2_GLFW_H
