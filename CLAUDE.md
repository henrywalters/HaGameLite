# HaGameLite (HaGame2)

The C++20 game engine behind metroid-shooter (SUBLEVEL) and the hg-editor library (hge). It builds one library,
`HaGame2`, which the games link as a prebuilt binary. A change here does nothing until the library is rebuilt.

Sibling repos are expected next to this one:

```
games/
  HaGameLite/       this repo, the engine
  hg-editor/        hge, the editor library (links HaGame2)
  metroid-shooter/  the game (links HaGame2 and hge)
```

## Building

Build in **Release** unless asked otherwise. The games choose `lib/` or `lib-debug/` from their own build type, so
the engine, hge and the game all need the same one.

| Directory | What it is |
|---|---|
| `lib/` | Release build directory. The library lands here (`libHaGame2.so` / `.a` on Linux) |
| `lib-debug/` | Debug build directory |
| `lib-asan/` | `-DHG_SANITIZE=ON` build, ASan + UBSan, used together with hge's and the game's asan builds |
| `wasm/` | Emscripten build |

```bash
cmake -S . -B lib -DCMAKE_BUILD_TYPE=Release     # once
cmake --build lib -j
```

`CMAKE_BUILD_TYPE` defaults to **Debug** when it isn't given, so always pass it.

### Third-party libraries

All of them are submodules in `thirdparty/`, except glad, which is checked in. Several are expected to be prebuilt in
place:

- `thirdparty/box2d/build/src/` holds box2d (v3), found with `find_library(box2d)`.
- `thirdparty/openal-soft/` holds the OpenAL library, built in its own folder (`libopenal.so` on Linux,
  `OpenAL32.dll`/`.lib` on Windows).
- `thirdparty/glfw/build/src/glfw3.lib` is used on Windows. Linux uses the system glfw3 through pkg-config.
- `thirdparty/assimp/build/lib/libassimp.a` is used on Linux only, plus system zlib.

After cloning, run `git submodule update --init --recursive`, then build box2d, openal-soft and glfw (Windows) in
those folders before configuring the engine.

## Things to know

- **Built-in shaders are compiled into the library.** `include/hagame/graphics/shaders/*.h` (TEXTURE_SHADER and the
  others) are compiled in `impl/common/scenes/loading.cpp`. Editing one needs an engine rebuild, then a game relink.
- **Components** derive from `hg::Component` and register with the `HG_COMPONENT(Category, Name)` and
  `HG_FIELD` / `HG_NUMBER_FIELD` macros, which drive saving, loading and the editor inspector. There are no enum
  fields, so games use strings.
- **Component memory stays put.** `hg::Component` sets `in_place_delete = true`, so `Entity::removeComponent<T>()`
  leaves other components' pointers valid. `Entity` keeps raw `Component*` in `m_components`, so don't remove this.
- **Physics2D** (`impl/physics/physics2D.cpp`) wraps Box2D v3, with no joints exposed.
  - Every fixed step it calls `setEnabled(entity->activeInHierarchy())` on every `RigidBody2D`. So a body turned off
    with `setEnabled(false)` comes back next step. To take a body out for good, remove the component.
  - `raycast` returns only the closest hit with fraction > 0. `Hit::entity` can be null, so check it.
  - Tilemap colliders are static chain bodies with no `RigidBody2D`.
  - Shapes map to entity **ids**, resolved through `entityForShape`, which also checks the entity still owns that
    exact shape. Entities get removed between steps (pickups, scripts), and Box2D reuses shape indices. Raw
    `Entity*`s kept across steps crashed on Windows, where freed memory gets reused sooner than on Linux.
    `RigidBody2D::collidingWith()` is pruned each step for the same reason.
- **Math:** `Mat4::get` isn't const, so take matrices by value when you only read them. `hg::math::rotate` exists, so
  avoid naming your own helper `rotate`.
- **Logging:** `hg::log::info / warn / error`. There's no `warning`.
- **Assets:** `hg::getPrefab(path)` / `hasPrefab`. A prefab's name is its path under `assets/` without the
  extension, such as `prefabs/impacts/metal`. `AssetWatcher` hot reloads changed files by polling mtimes through
  `std::filesystem`, so it works on any OS.

## Windows notes

The engine builds with MSVC from Visual Studio 2022 **17.5 or newer** (box2d v3 needs MSVC's C11 atomics), and
metroid-shooter runs on it. Build the third-party libraries with the Visual Studio generator in their own folders,
Release only:

```bash
# in thirdparty/box2d
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBOX2D_SAMPLES=OFF -DBOX2D_UNIT_TESTS=OFF -DBOX2D_BENCHMARKS=OFF
# in thirdparty/glfw
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
# in thirdparty/openal-soft. ALSOFT_INSTALL=ON is what writes build/OpenALConfig.cmake
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DALSOFT_EXAMPLES=OFF -DALSOFT_UTILS=OFF -DALSOFT_INSTALL=ON
# in thirdparty/assimp, with its bundled zlib
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_ZLIB=ON \
      -DASSIMP_BUILD_TESTS=OFF -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_INSTALL=OFF -DASSIMP_WARNINGS_AS_ERRORS=OFF
# then each: cmake --build build --config Release
```

Then build the engine from `build-vs/`:

```bash
cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64
cmake --build build-vs --config Release --target HaGame2
```

- **`HaGame2` is STATIC on Windows.** There's no `__declspec(dllexport)` anywhere, and the component and asset
  registries are static data, which `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS` can't export. A static engine doesn't pass its
  dependencies on when a game links it by path, so metroid-shooter's `cmake/FindHaGame2.cmake` lists them.
- **Output:** each configuration's library goes straight to `lib/HaGame2.lib` (Release and RelWithDebInfo) or
  `lib-debug/`, not the build folder's `Release/` subfolder.
- **MSVC flags:** `-Wall` is GCC/Clang only. The engine adds `NOMINMAX`, `_USE_MATH_DEFINES`, `_WIN32_WINNT`,
  `/bigobj`, `/utf-8` and `/Zc:__cplusplus`, and everything that includes its headers needs the same.
- **POSIX headers:** `unistd.h` and the like don't exist under MSVC, so guard them with `#ifndef _WIN32`.
  `utils/log.cpp` prints without colour on Windows.
- `utils/libLoader.cpp` uses `__UNIX__`, which no compiler defines. Windows takes the `_WIN32` `LoadLibrary` path,
  which is fine, but the Linux branch is dead code.
- `HaGame2.sln` / `.vcxproj` in the repo root are stale. Use CMake.

## Style

- 4-space indent, `hg::` namespaces, headers in `include/hagame/...`, sources in `impl/...`.
- Comments explain why, in full sentences.
- There's no test suite. Check changes by building the engine, then hge and the game, and running the game.
