//
// Created by henry on 12/16/22.
//

#include "../../include/hagame/graphics/window.h"
#include "../../include/hagame/input/input.h"
#include <iostream>

namespace hg::graphics {
    void GLAPIENTRY
    glErrorHandler(GLenum source,
                   GLenum type,
                   GLuint id,
                   GLenum severity,
                   GLsizei length,
                   const GLchar* message,
                   const void* userParam)
    {

        // ignore non-significant error/warning codes
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " <<  message << std::endl;

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }
}

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

    gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);

#ifdef __EMSCRIPTEN__
    std::cout << "IGNORING OPENGL DEBUG IN WASM\n";
#else
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glErrorHandler, 0);
#endif

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
    // Flip the double buffer
    glfwSwapBuffers(m_window);
    // Handle any events

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

void hg::graphics::Window::clear() {
    hg::input::Input::KeyboardMouse().clear();
    // Clear the window with the background color
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    // glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    //glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDepthFunc(GL_LESS);
}
