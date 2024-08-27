cmake_minimum_required(VERSION 3.24)
project(HaGame2)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/thirdparty/entt/")
set( OPENAL_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/thirdparty/openal-soft/")
set( AUDIOFILE_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/thirdparty/AudioFile/")

MESSAGE(${OPENAL_ROOT_DIR})

set(CMAKE_FIND_DEBUG_MODE FALSE)

find_package(PkgConfig REQUIRED)

if(NOT EMSCRIPTEN)
find_package(OpenAL CONFIG REQUIRED HINTS ${OPENAL_ROOT_DIR})
endif()

MESSAGE("OPENAL = ${OPENAL_LIBRARY}")

set(BOX2D_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty/box2d)
set(OPENAL_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty/openal-soft)

pkg_search_module(GLFW REQUIRED glfw3)

# add_subdirectory(${BOX2D_ROOT_DIR})

MESSAGE("INCLUDE DIRS = ${GLFW_INCLUDE_DIRS}")
MESSAGE("LIBS = ${GLFW_LIBRARIES}")

add_library(HaGame2 SHARED impl/core/component.cpp include/hagame/math/vector.h include/hagame/math/constants.h include/hagame/math/aliases.h include/hagame/utils/clock.h include/hagame/utils/timer.h include/hagame/math/matrix.h include/hagame/math/quaternion.h include/hagame/math/hypercube.h include/hagame/graphics/window.h impl/graphics/window.cpp include/hagame/graphics/resolution.h include/hagame/graphics/color.h include/hagame/graphics/rgb.h include/hagame/graphics/hsv.h include/hagame/input/devices/keyboardMouse.h include/hagame/input/input.h include/hagame/input/inputDevice.h include/hagame/graphics/glfw.h include/hagame/graphics/shader.h impl/graphics/shader.cpp thirdparty/glad/glad.h thirdparty/glad/glad.c include/hagame/graphics/shaderProgram.h impl/graphics/shaderProgram.cpp include/hagame/graphics/vertexArray.h include/hagame/graphics/dataTypes.h include/hagame/math/transform.h impl/physics/transform.cpp include/hagame/math/conversions.h include/hagame/graphics/mesh.h impl/graphics/mesh.cpp include/hagame/graphics/vertex.h include/hagame/graphics/primitives/quad.h include/hagame/graphics/triangle.h include/hagame/math/triangle.h impl/graphics/primitives/quad.cpp include/hagame/utils/file.h impl/utils/file.cpp include/hagame/utils/string.h impl/utils/string.cpp impl/graphics/primitives/cube.cpp include/hagame/graphics/primitives/cube.h impl/graphics/primitives/disc.cpp include/hagame/graphics/primitives/disc.h include/hagame/utils/macros.h include/hagame/core/gameObject.h include/hagame/core/object.h include/hagame/utils/uuid.h impl/core/gameObject.cpp include/hagame/utils/pubsub.h include/hagame/utils/random.h include/hagame/graphics/windows.h impl/graphics/windows.cpp include/hagame/graphics/font.h impl/graphics/font.cpp impl/graphics/text.cpp include/hagame/graphics/text.h include/hagame/graphics/primitives/line.h impl/graphics/primitives/line.cpp include/hagame/math/lineIntersection.h include/hagame/math/interval.h include/hagame/math/lineSegment.h include/hagame/core/scene.h include/hagame/utils/stateMachine.h include/hagame/core/game.h include/hagame/graphics/renderPipeline.h include/hagame/core/hg.h impl/core/hg.cpp impl/core/game.cpp include/hagame/graphics/frameBuffer.h include/hagame/utils/store.h include/hagame/graphics/rawTexture.h include/hagame/graphics/texture.h include/hagame/graphics/image.h impl/graphics/image.cpp impl/graphics/texture.cpp include/hagame/graphics/renderPasses.h include/hagame/math/ray.h include/hagame/math/line.h include/hagame/math/collisions.h include/hagame/math/hypersphere.h include/hagame/utils/loopingThread.h include/hagame/core/component.h include/hagame/utils/config.h include/hagame/core/script.h impl/utils/config.cpp impl/core/scene.cpp include/hagame/graphics/components/sprite.h include/hagame/math/functions.h include/hagame/net/timer.h include/hagame/net/message.h include/hagame/net/net.h impl/net/net.cpp impl/net/timer.cpp include/hagame/net/server.h include/hagame/net/client.h include/hagame/core/assets.h impl/core/assets.cpp include/hagame/utils/hash.h include/hagame/utils/spatialMap.h include/hagame/graphics/camera.h include/hagame/graphics/components/quad.h include/hagame/graphics/tilemap.h impl/graphics/tilemap.cpp include/hagame/math/polygon.h include/hagame/common/components/topDownPlayerController.h include/hagame/utils/parameters.h "impl/ math/collisions.cpp" include/hagame/graphics/debug.h impl/graphics/debug.cpp include/hagame/graphics/particleEmitter.h impl/graphics/particleEmitter.cpp include/hagame/graphics/components/particleEmitterComponent.h impl/graphics/components/particleEmitterComponent.cpp include/hagame/utils/components/timeout.h impl/utils/components/timeout.cpp include/hagame/common/weapons.h impl/common/weapons.cpp include/hagame/utils/pathfinding.h impl/utils/pathfinding.cpp include/hagame/interfaces/iNodeMap.h include/hagame/core/entityMap.h impl/core/entityMap.cpp include/hagame/common/components/healthBar.h include/hagame/utils/watcher.h include/hagame/utils/profiler.h impl/utils/profiler.cpp include/hagame/core/groupManager.h impl/core/groupManager.cpp include/hagame/structures/binaryHeap.h include/hagame/structures/comparable.h include/hagame/structures/valued.h include/hagame/core/system.h include/hagame/graphics/textBuffer.h impl/graphics/textBuffer.cpp include/hagame/graphics/shaders/text.h include/hagame/utils/behaviorTree.h impl/utils/behaviorTree.cpp include/hagame/utils/helpers.h include/hagame/utils/steadyRate.h include/hagame/math/components/rectCollider.h include/hagame/math/components/circleCollider.h "impl/ math/functions.cpp" include/hagame/utils/variant.h impl/utils/variant.cpp include/hagame/math/components/collider.h "impl/ math/collisions/collider.cpp" include/hagame/graphics/components/textRenderer.h include/hagame/graphics/spriteSheet.h impl/graphics/spriteSheet.cpp include/hagame/graphics/primitives/grid.h impl/graphics/primitives/grid.cpp include/hagame/graphics/shaders/texture.h include/hagame/graphics/components/spriteSheetAnimator.h include/hagame/structures/graph.h include/hagame/graphics/animation.h include/hagame/graphics/shaders/color.h include/hagame/graphics/shaders/particle.h include/hagame/common/level2D.h include/hagame/audio/device.h impl/audio/device.cpp include/hagame/audio/stream.h impl/audio/stream.cpp include/hagame/audio/player.h impl/audio/player.cpp include/hagame/audio/source.h include/hagame/audio/source.h include/hagame/structures/tsqueue.h include/hagame/audio/components/listener.h include/hagame/audio/components/source.h include/hagame/audio/streamBuffer.h impl/audio/streamBuffer.cpp include/hagame/graphics/aspectRatios.h "impl/ math/lineIntersection.cpp" include/hagame/graphics/primitives/light.h impl/graphics/primitives/light.cpp impl/ui/anchor.cpp include/hagame/ui/anchor.h include/hagame/ui/element.h include/hagame/ui/elements/button.h impl/ui/elements/button.cpp include/hagame/ui/elements/textButton.h impl/ui/elements/textButton.cpp include/hagame/graphics/batchRenderer.h impl/graphics/batchRenderer.cpp impl/core/system.cpp include/hagame/core/cppScript.h impl/core/cppScript.cpp impl/core/script.cpp impl/graphics/monitors.cpp include/hagame/graphics/components/tilemap.h impl/graphics/components/tilemap.cpp include/hagame/utils/enum.h impl/utils/enum.cpp include/hagame/ui/elements/container.h include/hagame/input/devices/gamepad.h impl/input/input.cpp impl/input/devices/gamepad.cpp impl/input/devices/inputDevice.cpp impl/input/devices/keyboardMouse.cpp include/hagame/input/deviceManager.h impl/input/deviceManager.cpp include/hagame/input/action.h include/hagame/input/devices/keyboardMouseEnums.h include/hagame/ui/events.h impl/ui/element.cpp include/hagame/structures/tree.h impl/core/entity.cpp include/hagame/common/console.h impl/common/console.cpp include/hagame/graphics/textureAtlas.h include/hagame/graphics/animatedSprite.h impl/graphics/textureAtlas.cpp impl/graphics/animatedSprite.cpp include/hagame/graphics/spriteSheetV2.h impl/graphics/spriteSheetV2.cpp include/hagame/common/scenes/loading.h impl/common/scenes/loading.cpp include/hagame/utils/variantMap.h include/hagame/ui/style.h include/hagame/ui/elements/gridContainer.h impl/ui/elements/gridContainer.cpp impl/ui/elements/container.cpp include/hagame/ui/frame.h impl/ui/frame.cpp include/hagame/ui/elements/label.h impl/ui/elements/label.cpp impl/ui/mixins/textDisplay.cpp include/hagame/ui/mixins/textDisplay.h include/hagame/ui/graphicsContext.h impl/ui/graphicsContext.cpp include/hagame/ui/elements/checkbox.h impl/ui/elements/checkbox.cpp impl/graphics/primitives/quadBorder.cpp include/hagame/graphics/primitives/quadBorder.h include/hagame/ui/elements/divider.h impl/ui/elements/divider.cpp include/hagame/ui/elements/image.h impl/ui/elements/image.cpp)

target_include_directories(HaGame2 PRIVATE ${AUDIOFILE_ROOT_DIR})
target_include_directories(HaGame2 PRIVATE ${BOX2D_ROOT_DIR}/include)
target_include_directories(HaGame2 PRIVATE ${GLFW_INCLUDE_DIRS})
target_include_directories(HaGame2 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/thirdparty/entt/src/")
target_include_directories(HaGame2 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/thirdparty/stb/")
target_include_directories(HaGame2 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/thirdparty/asio/asio/include/")

if(NOT EMSCRIPTEN)
    target_include_directories(HaGame2 PRIVATE ${OPENAL_INCLUDE_DIR})
endif()


target_link_libraries(HaGame2 PUBLIC ${GLFW_LIBRARIES})
# target_link_libraries(HaGame2 PUBLIC Box2D)
target_link_libraries(HaGame2 PUBLIC ${OPENAL_LIBRARY})

if(EMSCRIPTEN)
    set_target_properties(HaGame2 PROPERTIES LINK_FLAGS "-DCMAKE_EXE_LINKER_FLAGS=-s -sMAX_WEBGL_VERSION=2 -sUSE_GLFW -lopenal -sSTACK_SIZE=1000000")
endif()