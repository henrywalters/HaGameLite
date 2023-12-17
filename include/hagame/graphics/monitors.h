//
// Created by henry on 12/16/23.
//

#ifndef HAGAME2_MONITORS_H
#define HAGAME2_MONITORS_H

#include "../math/aliases.h"
#include "glfw.h"

namespace hg::graphics {

    struct VideoMode {
        int refreshRate;
        Vec2i size;
        int redBits;
        int greenBits;
        int blueBits;
    };

    struct Monitor {
        GLFWmonitor* monitor;
        std::vector<VideoMode> videoModes;
        std::string name;
        Vec2i physicalSize;
        Vec2 scale;
        Vec2i pos;
        Recti workArea;
    };

    class Monitors {
    public:
        static std::vector<Monitor> All();
    };
}



#endif //HAGAME2_MONITORS_H
