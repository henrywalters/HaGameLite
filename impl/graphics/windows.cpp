//
// Created by henry on 12/29/22.
//

#include "../../include/hagame/graphics/windows.h"

hg::Publisher<hg::graphics::WindowEvents, hg::graphics::WindowEvent> hg::graphics::Windows::Events = hg::Publisher<hg::graphics::WindowEvents, hg::graphics::WindowEvent>();
std::unordered_map<GLFWwindow*, std::shared_ptr<hg::graphics::Window>> hg::graphics::Windows::s_windows = std::unordered_map<GLFWwindow*, std::shared_ptr<Window>>();

void hg::graphics::Windows::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    s_windows[window]->input.keyboardMouse.keyCallback(key, action);
}

void hg::graphics::Windows::MouseCursorPosCallback(GLFWwindow *window, double xPos, double yPos) {
    s_windows[window]->input.keyboardMouse.cursorPosCallback(xPos, yPos);
}

void hg::graphics::Windows::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    s_windows[window]->input.keyboardMouse.mouseButtonCallback(button, action);
}

void hg::graphics::Windows::MouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    s_windows[window]->input.keyboardMouse.scrollCallback(xOffset, yOffset);
}

void hg::graphics::Windows::CloseCallback(GLFWwindow *window) {
    Windows::Events.emit(WindowEvents::Close, s_windows[window].get());
}

void hg::graphics::Windows::ResizeCallback(GLFWwindow *window, int width, int height) {
    s_windows[window]->m_size = Resolution(width, height);
    Windows::Events.emit(WindowEvents::Resize, s_windows[window].get());
}

hg::graphics::Window *hg::graphics::Windows::Create(std::string title, Resolution size) {
    auto window = std::make_shared<Window>(title, size);
    window->initialize();
    s_windows.insert(std::make_pair(window->window(), window));
    glfwSetKeyCallback(window->window(), KeyCallback);
    glfwSetCursorPosCallback(window->window(), MouseCursorPosCallback);
    glfwSetMouseButtonCallback(window->window(), MouseButtonCallback);
    glfwSetScrollCallback(window->window(), MouseScrollCallback);
    glfwSetWindowCloseCallback(window->window(), CloseCallback);
    glfwSetWindowSizeCallback(window->window(), ResizeCallback);
    glfwSetCharCallback(window->window(), CharCallback);
    return window.get();
}

void hg::graphics::Windows::Destroy(hg::graphics::Window *window) {
    s_windows.erase(window->window());
    window->close();
}

std::vector<hg::graphics::Window *> hg::graphics::Windows::All() {
    std::vector<Window*> windows;
    for (const auto& [id, window] : s_windows) {
        windows.push_back(window.get());
    }
    return windows;
}

void hg::graphics::Windows::CharCallback(GLFWwindow *window, unsigned int codepoint) {
    s_windows[window]->input.keyboardMouse.charCallback(codepoint);
}

