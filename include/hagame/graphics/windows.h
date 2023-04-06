//
// Created by henry on 12/29/22.
//

#ifndef HAGAME2_WINDOWS_H
#define HAGAME2_WINDOWS_H

#include "window.h"

namespace hg::graphics {
    enum class WindowEvents {
        Resize,
        Close,
    };

    using WindowEvent = Window*;

    class Windows {
    public:

        static Publisher<WindowEvents, WindowEvent> Events;

        static Window* Create(std::string title, Resolution size = HD);

        static void Destroy(Window* window);

        static std::vector<Window*> All();

    private:

        static std::unordered_map<GLFWwindow*, std::shared_ptr<Window>> s_windows;

        static void CloseCallback(GLFWwindow* window);

        static void ResizeCallback(GLFWwindow* window, int width, int height);

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void MouseCursorPosCallback(GLFWwindow* window, double xPos, double yPos);

    };
}

#endif //HAGAME2_WINDOWS_H
