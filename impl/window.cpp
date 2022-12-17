//
// Created by henry on 12/16/22.
//

#include "../include/hagame/graphics/window.h"
#include "../include/hagame/input/input.h"
#include <iostream>

hg::graphics::Window::Window(std::string _title, Vec2i _size) {
    m_title = _title;
    m_size = _size;
}

void hg::graphics::Window::initialize() {

    glfwSetErrorCallback(Window::ErrorCallback);

    if (!glfwInit()) {
        fprintf(stderr, "Error: GLFW Initialization failed.");
        Window::ForceExit();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    m_window = glfwCreateWindow(m_size[0], m_size[1], m_title.c_str(), NULL, NULL);

    if (!m_window) {
        fprintf(stderr, "Error: GLFW Window Creation Failed");
        glfwTerminate();
        ForceExit();
    }

    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetCursorPosCallback(m_window, MouseCursorPosCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);

    glfwMakeContextCurrent(m_window);
}

void hg::graphics::Window::ForceExit() {
    {
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#else
        exit(EXIT_FAILURE);
#endif
    }
}

void hg::graphics::Window::ErrorCallback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void hg::graphics::Window::close() {
    // Clean up
    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void hg::graphics::Window::render() {
    // Clear the window with the background color
    glClear(GL_COLOR_BUFFER_BIT);
    // Flip the double buffer
    glfwSwapBuffers(m_window);
    // Handle any events

    hg::input::Input::KeyboardMouse().clear();

    glfwPollEvents();
}

void hg::graphics::Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    hg::input::Input::KeyboardMouse().keyCallback(key, action);
}

void hg::graphics::Window::MouseCursorPosCallback(GLFWwindow *window, double xPos, double yPos) {
    hg::input::Input::KeyboardMouse().cursorPosCallback(xPos, yPos);
}

void hg::graphics::Window::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    hg::input::Input::KeyboardMouse().mouseButtonCallback(button, action);
}
