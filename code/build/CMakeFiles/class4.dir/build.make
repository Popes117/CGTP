# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build"

# Include any dependencies generated for this target.
include CMakeFiles/class4.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/class4.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/class4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/class4.dir/flags.make

CMakeFiles/class4.dir/engine.cpp.o: CMakeFiles/class4.dir/flags.make
CMakeFiles/class4.dir/engine.cpp.o: /home/popes/Desktop/3ºAno\ 2ºSemestre/cg24/code/engine.cpp
CMakeFiles/class4.dir/engine.cpp.o: CMakeFiles/class4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/class4.dir/engine.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/class4.dir/engine.cpp.o -MF CMakeFiles/class4.dir/engine.cpp.o.d -o CMakeFiles/class4.dir/engine.cpp.o -c "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/engine.cpp"

CMakeFiles/class4.dir/engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/class4.dir/engine.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/engine.cpp" > CMakeFiles/class4.dir/engine.cpp.i

CMakeFiles/class4.dir/engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/class4.dir/engine.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/engine.cpp" -o CMakeFiles/class4.dir/engine.cpp.s

# Object files for target class4
class4_OBJECTS = \
"CMakeFiles/class4.dir/engine.cpp.o"

# External object files for target class4
class4_EXTERNAL_OBJECTS =

class4: CMakeFiles/class4.dir/engine.cpp.o
class4: CMakeFiles/class4.dir/build.make
class4: /usr/lib/libGL.so
class4: /usr/lib/libGLU.so
class4: /usr/lib/libglut.so
class4: /usr/lib/libXmu.so
class4: /usr/lib/libXi.so
class4: /usr/lib/libGLEW.so
class4: /usr/lib/libGL.so
class4: /usr/lib/libGLU.so
class4: /usr/lib/libglut.so
class4: /usr/lib/libXmu.so
class4: /usr/lib/libXi.so
class4: /usr/lib/libGLEW.so
class4: CMakeFiles/class4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable class4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/class4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/class4.dir/build: class4
.PHONY : CMakeFiles/class4.dir/build

CMakeFiles/class4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/class4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/class4.dir/clean

CMakeFiles/class4.dir/depend:
	cd "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code" "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code" "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build" "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build" "/home/popes/Desktop/3ºAno 2ºSemestre/cg24/code/build/CMakeFiles/class4.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/class4.dir/depend

