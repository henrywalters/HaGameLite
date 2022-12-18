#include "../include/hagame.h"
#include "../include/hagame/math/aliases.h"
#include "../include/hagame/utils/timer.h"
#include "../include/hagame/math/quaternion.h"
#include "../include/hagame/math/matrix.h"
#include "../include/hagame/math/hypercube.h"
// #include "../include/hagame/graphics/window.h"
#include "../impl/window.cpp"
#include "../impl/shader.cpp"
#include "../impl/shaderProgram.cpp"

// Include the Emscripten library only if targetting WebAssembly
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#endif

#include <iostream>
#include <GLFW/glfw3.h>

void hello() {

    hg::Vec3 a;

    hg::utils::Timer timer;

    for (int i = 1; i < 1000000; i++) {
        a += hg::Vec3(std::log(i));
    }

    std::cout << "A = " << a << "\n";
    std::cout << "Elapsed time: " << hg::utils::Clock::ToSeconds(timer.elapsed()) << "\n";


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}
