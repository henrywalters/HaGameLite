# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/henry/CLionProjects/HaGame2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/henry/CLionProjects/HaGame2/public

# Include any dependencies generated for this target.
include CMakeFiles/HaGame2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HaGame2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HaGame2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HaGame2.dir/flags.make

CMakeFiles/HaGame2.dir/impl/hagame.cpp.o: CMakeFiles/HaGame2.dir/flags.make
CMakeFiles/HaGame2.dir/impl/hagame.cpp.o: /home/henry/CLionProjects/HaGame2/impl/hagame.cpp
CMakeFiles/HaGame2.dir/impl/hagame.cpp.o: CMakeFiles/HaGame2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/CLionProjects/HaGame2/public/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HaGame2.dir/impl/hagame.cpp.o"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HaGame2.dir/impl/hagame.cpp.o -MF CMakeFiles/HaGame2.dir/impl/hagame.cpp.o.d -o CMakeFiles/HaGame2.dir/impl/hagame.cpp.o -c /home/henry/CLionProjects/HaGame2/impl/hagame.cpp

CMakeFiles/HaGame2.dir/impl/hagame.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HaGame2.dir/impl/hagame.cpp.i"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/CLionProjects/HaGame2/impl/hagame.cpp > CMakeFiles/HaGame2.dir/impl/hagame.cpp.i

CMakeFiles/HaGame2.dir/impl/hagame.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HaGame2.dir/impl/hagame.cpp.s"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/CLionProjects/HaGame2/impl/hagame.cpp -o CMakeFiles/HaGame2.dir/impl/hagame.cpp.s

CMakeFiles/HaGame2.dir/impl/window.cpp.o: CMakeFiles/HaGame2.dir/flags.make
CMakeFiles/HaGame2.dir/impl/window.cpp.o: /home/henry/CLionProjects/HaGame2/impl/window.cpp
CMakeFiles/HaGame2.dir/impl/window.cpp.o: CMakeFiles/HaGame2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/CLionProjects/HaGame2/public/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HaGame2.dir/impl/window.cpp.o"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HaGame2.dir/impl/window.cpp.o -MF CMakeFiles/HaGame2.dir/impl/window.cpp.o.d -o CMakeFiles/HaGame2.dir/impl/window.cpp.o -c /home/henry/CLionProjects/HaGame2/impl/window.cpp

CMakeFiles/HaGame2.dir/impl/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HaGame2.dir/impl/window.cpp.i"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/CLionProjects/HaGame2/impl/window.cpp > CMakeFiles/HaGame2.dir/impl/window.cpp.i

CMakeFiles/HaGame2.dir/impl/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HaGame2.dir/impl/window.cpp.s"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/CLionProjects/HaGame2/impl/window.cpp -o CMakeFiles/HaGame2.dir/impl/window.cpp.s

CMakeFiles/HaGame2.dir/impl/shader.cpp.o: CMakeFiles/HaGame2.dir/flags.make
CMakeFiles/HaGame2.dir/impl/shader.cpp.o: /home/henry/CLionProjects/HaGame2/impl/shader.cpp
CMakeFiles/HaGame2.dir/impl/shader.cpp.o: CMakeFiles/HaGame2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/CLionProjects/HaGame2/public/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/HaGame2.dir/impl/shader.cpp.o"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HaGame2.dir/impl/shader.cpp.o -MF CMakeFiles/HaGame2.dir/impl/shader.cpp.o.d -o CMakeFiles/HaGame2.dir/impl/shader.cpp.o -c /home/henry/CLionProjects/HaGame2/impl/shader.cpp

CMakeFiles/HaGame2.dir/impl/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HaGame2.dir/impl/shader.cpp.i"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/CLionProjects/HaGame2/impl/shader.cpp > CMakeFiles/HaGame2.dir/impl/shader.cpp.i

CMakeFiles/HaGame2.dir/impl/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HaGame2.dir/impl/shader.cpp.s"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/CLionProjects/HaGame2/impl/shader.cpp -o CMakeFiles/HaGame2.dir/impl/shader.cpp.s

CMakeFiles/HaGame2.dir/impl/glad.c.o: CMakeFiles/HaGame2.dir/flags.make
CMakeFiles/HaGame2.dir/impl/glad.c.o: /home/henry/CLionProjects/HaGame2/impl/glad.c
CMakeFiles/HaGame2.dir/impl/glad.c.o: CMakeFiles/HaGame2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/CLionProjects/HaGame2/public/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/HaGame2.dir/impl/glad.c.o"
	/home/henry/emsdk/upstream/emscripten/emcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/HaGame2.dir/impl/glad.c.o -MF CMakeFiles/HaGame2.dir/impl/glad.c.o.d -o CMakeFiles/HaGame2.dir/impl/glad.c.o -c /home/henry/CLionProjects/HaGame2/impl/glad.c

CMakeFiles/HaGame2.dir/impl/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/HaGame2.dir/impl/glad.c.i"
	/home/henry/emsdk/upstream/emscripten/emcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/henry/CLionProjects/HaGame2/impl/glad.c > CMakeFiles/HaGame2.dir/impl/glad.c.i

CMakeFiles/HaGame2.dir/impl/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/HaGame2.dir/impl/glad.c.s"
	/home/henry/emsdk/upstream/emscripten/emcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/henry/CLionProjects/HaGame2/impl/glad.c -o CMakeFiles/HaGame2.dir/impl/glad.c.s

CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o: CMakeFiles/HaGame2.dir/flags.make
CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o: /home/henry/CLionProjects/HaGame2/impl/shaderProgram.cpp
CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o: CMakeFiles/HaGame2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/CLionProjects/HaGame2/public/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o -MF CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o.d -o CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o -c /home/henry/CLionProjects/HaGame2/impl/shaderProgram.cpp

CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.i"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/CLionProjects/HaGame2/impl/shaderProgram.cpp > CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.i

CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.s"
	/home/henry/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/CLionProjects/HaGame2/impl/shaderProgram.cpp -o CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.s

# Object files for target HaGame2
HaGame2_OBJECTS = \
"CMakeFiles/HaGame2.dir/impl/hagame.cpp.o" \
"CMakeFiles/HaGame2.dir/impl/window.cpp.o" \
"CMakeFiles/HaGame2.dir/impl/shader.cpp.o" \
"CMakeFiles/HaGame2.dir/impl/glad.c.o" \
"CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o"

# External object files for target HaGame2
HaGame2_EXTERNAL_OBJECTS =

libHaGame2.a: CMakeFiles/HaGame2.dir/impl/hagame.cpp.o
libHaGame2.a: CMakeFiles/HaGame2.dir/impl/window.cpp.o
libHaGame2.a: CMakeFiles/HaGame2.dir/impl/shader.cpp.o
libHaGame2.a: CMakeFiles/HaGame2.dir/impl/glad.c.o
libHaGame2.a: CMakeFiles/HaGame2.dir/impl/shaderProgram.cpp.o
libHaGame2.a: CMakeFiles/HaGame2.dir/build.make
libHaGame2.a: CMakeFiles/HaGame2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/CLionProjects/HaGame2/public/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libHaGame2.a"
	$(CMAKE_COMMAND) -P CMakeFiles/HaGame2.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HaGame2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HaGame2.dir/build: libHaGame2.a
.PHONY : CMakeFiles/HaGame2.dir/build

CMakeFiles/HaGame2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HaGame2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HaGame2.dir/clean

CMakeFiles/HaGame2.dir/depend:
	cd /home/henry/CLionProjects/HaGame2/public && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/CLionProjects/HaGame2 /home/henry/CLionProjects/HaGame2 /home/henry/CLionProjects/HaGame2/public /home/henry/CLionProjects/HaGame2/public /home/henry/CLionProjects/HaGame2/public/CMakeFiles/HaGame2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HaGame2.dir/depend

