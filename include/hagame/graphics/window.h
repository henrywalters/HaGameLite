//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_WINDOW_H
#define HAGAME2_WINDOW_H

#include "glfw.h"
#include "../math/aliases.h"
#include "resolution.h"
#include "color.h"
#include "../utils/pubsub.h"
#include "../input/input.h"

namespace hg::graphics {

    class Windows;

    class Window {
    public:

        input::Input input;

        // Sized window
        Window(std::string _title, Resolution _size = HD);

        void initialize();
        void clear();
        void render();
        void close();

        void title(std::string _title) {
            m_title = _title;
            glfwSetWindowTitle(m_window, m_title.c_str());
        }
        std::string title() { return m_title; }

        void size(Resolution _size) {
            m_size = _size;
            glfwSetWindowSize(m_window, m_size[0], m_size[1]);
            glViewport(0, 0, m_size[0], m_size[1]);
        }
        Resolution size() { return m_size; }

        void color(Color _color) {
            m_color = _color;
            glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]);
        }
        Color color() { return m_color; }

        void running(bool _running) {
            m_running = _running;
            if (!running()) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
        }
        bool running() { return m_running; }

        GLFWwindow* window() const { return m_window; }

        void setVSync(bool vsyncOn) {
            glfwSwapInterval((int) vsyncOn);
        }

        void setMouseVisible(bool visible) {
            glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }

    private:

        friend class Windows;

        GLFWwindow* m_window;

        std::string m_title;
        Resolution m_size;
        Color m_color = Color::black();
        bool m_running = true;

        // Handle GLFW Errors
        static void ErrorCallback(int error, const char* description);

        static void ForceExit();
    };
}

#endif //HAGAME2_WINDOW_H
