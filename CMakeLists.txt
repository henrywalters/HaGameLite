cmake_minimum_required(VERSION 3.24)
project(HaGame2)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

MESSAGE("INCLUDE DIRS = ${GLFW_INCLUDE_DIRS}")
MESSAGE("LIBS = ${GLFW_LIBRARIES}")

include_directories(${GLFW_INCLUDE_DIRS})
add_library(HaGame2 impl/hagame.cpp include/hagame/math/vector.h include/hagame/math/constants.h include/hagame/math/aliases.h include/hagame/utils/clock.h include/hagame/utils/timer.h include/hagame/math/matrix.h include/hagame/math/quaternion.h include/hagame/math/hypercube.h include/hagame/graphics/window.h impl/window.cpp include/hagame/graphics/resolution.h include/hagame/graphics/color.h include/hagame/graphics/rgb.h include/hagame/graphics/hsv.h include/hagame/input/devices/keyboardMouse.h include/hagame/input/input.h include/hagame/input/inputDevice.h include/hagame/graphics/glfw.h include/hagame/graphics/shader.h impl/shader.cpp thirdparty/glad.h thirdparty/glad.c include/hagame/graphics/shaderProgram.h impl/shaderProgram.cpp include/hagame/graphics/buffer.h include/hagame/graphics/vertexArray.h include/hagame/graphics/dataTypes.h)
target_link_libraries(HaGame2 ${GLFW_LIBRARIES})