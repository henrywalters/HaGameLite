cmake_minimum_required(VERSION 3.24)
project(HaGame2)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/thirdparty/entt/")

set(CMAKE_FIND_DEBUG_MODE TRUE)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories("${CMAKE_CURRENT_LIST_DIR}/thirdparty/entt/src/")
include_directories("${CMAKE_CURRENT_LIST_DIR}/thirdparty/stb/")

MESSAGE("INCLUDE DIRS = ${GLFW_INCLUDE_DIRS}")
MESSAGE("LIBS = ${GLFW_LIBRARIES}")

include_directories(${GLFW_INCLUDE_DIRS})
add_library(HaGame2 impl/hagame.cpp include/hagame/math/vector.h include/hagame/math/constants.h include/hagame/math/aliases.h include/hagame/utils/clock.h include/hagame/utils/timer.h include/hagame/math/matrix.h include/hagame/math/quaternion.h include/hagame/math/hypercube.h include/hagame/graphics/window.h impl/graphics/window.cpp include/hagame/graphics/resolution.h include/hagame/graphics/color.h include/hagame/graphics/rgb.h include/hagame/graphics/hsv.h include/hagame/input/devices/keyboardMouse.h include/hagame/input/input.h include/hagame/input/inputDevice.h include/hagame/graphics/glfw.h include/hagame/graphics/shader.h impl/graphics/shader.cpp thirdparty/glad.h thirdparty/glad.c include/hagame/graphics/shaderProgram.h impl/graphics/shaderProgram.cpp include/hagame/graphics/buffer.h include/hagame/graphics/vertexArray.h include/hagame/graphics/dataTypes.h include/hagame/math/transform.h impl/physics/transform.cpp include/hagame/math/conversions.h include/hagame/graphics/mesh.h impl/graphics/mesh.cpp include/hagame/graphics/vertex.h include/hagame/graphics/primitives/quad.h include/hagame/graphics/triangle.h include/hagame/math/triangle.h impl/graphics/primitives/quad.cpp include/hagame/utils/file.h impl/utils/file.cpp include/hagame/utils/string.h impl/utils/string.cpp impl/graphics/primitives/cube.cpp include/hagame/graphics/primitives/cube.h impl/graphics/primitives/disc.cpp include/hagame/graphics/primitives/disc.h include/hagame/utils/macros.h include/hagame/core/gameObject.h include/hagame/core/object.h include/hagame/utils/uuid.h impl/core/gameObject.cpp include/hagame/utils/pubsub.h include/hagame/utils/random.h include/hagame/graphics/windows.h impl/graphics/windows.cpp include/hagame/graphics/font.h impl/graphics/font.cpp impl/graphics/text.cpp include/hagame/graphics/text.h include/hagame/graphics/primitives/line.h impl/graphics/primitives/line.cpp include/hagame/math/lineIntersection.h include/hagame/math/interval.h include/hagame/math/lineSegment.h include/hagame/core/scene.h include/hagame/utils/stateMachine.h include/hagame/core/game.h include/hagame/graphics/renderPipeline.h include/hagame/core/hg.h impl/core/hg.cpp impl/core/game.cpp include/hagame/graphics/frameBuffer.h include/hagame/utils/store.h include/hagame/graphics/rawTexture.h include/hagame/graphics/texture.h include/hagame/graphics/image.h impl/graphics/image.cpp impl/graphics/texture.cpp include/hagame/graphics/renderPasses.h include/hagame/math/ray.h include/hagame/math/line.h include/hagame/math/collisions.h include/hagame/math/hypersphere.h include/hagame/utils/loopingThread.h include/hagame/core/component.h include/hagame/utils/configParser.h)
target_link_libraries(HaGame2 ${GLFW_LIBRARIES})