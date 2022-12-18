//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_WINDOW_H
#define HAGAME2_WINDOW_H

#include "glfw.h"
#include "../math/aliases.h"
#include "resolution.h"
#include "color.h"

namespace hg::graphics {

    class Window {
    public:
        // Sized window
        Window(std::string _title, Resolution _size = HD);

        void initialize();
        void clear();
        void render();
        void close();

        void setTitle(std::string _title) {
            m_title = _title;
            glfwSetWindowTitle(m_window, m_title.c_str());
        }
        std::string getTitle() { return m_title; }

        void setSize(Resolution _size) {
            m_size = _size;
            glfwSetWindowSize(m_window, m_size[0], m_size[1]);
            glViewport(0, 0, m_size[0], m_size[1]);
        }
        Resolution getSize() { return m_size; }

        void setColor(Color _color) {
            m_color = _color;
            glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]);
        }
        Color getColor() { return m_color; }

        void setRunning(bool _running) {
            m_running = _running;
            if (!getRunning()) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
        }
        bool getRunning() { return m_running; }
    private:

        GLFWwindow* m_window;

        std::string m_title;
        Resolution m_size;
        Color m_color = Color::black();
        bool m_running = true;

        // Handle GLFW Errors
        static void ErrorCallback(int error, const char* description);

        static void ForceExit();

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void MouseCursorPosCallback(GLFWwindow* window, double xPos, double yPos);
    };
}

#endif //HAGAME2_WINDOW_H
