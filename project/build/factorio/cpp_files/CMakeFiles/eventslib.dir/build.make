# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build

# Include any dependencies generated for this target.
include factorio/cpp_files/CMakeFiles/eventslib.dir/depend.make

# Include the progress variables for this target.
include factorio/cpp_files/CMakeFiles/eventslib.dir/progress.make

# Include the compile flags for this target's objects.
include factorio/cpp_files/CMakeFiles/eventslib.dir/flags.make

factorio/cpp_files/CMakeFiles/eventslib.dir/events.cpp.o: factorio/cpp_files/CMakeFiles/eventslib.dir/flags.make
factorio/cpp_files/CMakeFiles/eventslib.dir/events.cpp.o: ../factorio/cpp_files/events.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object factorio/cpp_files/CMakeFiles/eventslib.dir/events.cpp.o"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/eventslib.dir/events.cpp.o -c /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files/events.cpp

factorio/cpp_files/CMakeFiles/eventslib.dir/events.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eventslib.dir/events.cpp.i"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files/events.cpp > CMakeFiles/eventslib.dir/events.cpp.i

factorio/cpp_files/CMakeFiles/eventslib.dir/events.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eventslib.dir/events.cpp.s"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files/events.cpp -o CMakeFiles/eventslib.dir/events.cpp.s

# Object files for target eventslib
eventslib_OBJECTS = \
"CMakeFiles/eventslib.dir/events.cpp.o"

# External object files for target eventslib
eventslib_EXTERNAL_OBJECTS =

factorio/cpp_files/libeventslib.a: factorio/cpp_files/CMakeFiles/eventslib.dir/events.cpp.o
factorio/cpp_files/libeventslib.a: factorio/cpp_files/CMakeFiles/eventslib.dir/build.make
factorio/cpp_files/libeventslib.a: factorio/cpp_files/CMakeFiles/eventslib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libeventslib.a"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && $(CMAKE_COMMAND) -P CMakeFiles/eventslib.dir/cmake_clean_target.cmake
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/eventslib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
factorio/cpp_files/CMakeFiles/eventslib.dir/build: factorio/cpp_files/libeventslib.a

.PHONY : factorio/cpp_files/CMakeFiles/eventslib.dir/build

factorio/cpp_files/CMakeFiles/eventslib.dir/clean:
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && $(CMAKE_COMMAND) -P CMakeFiles/eventslib.dir/cmake_clean.cmake
.PHONY : factorio/cpp_files/CMakeFiles/eventslib.dir/clean

factorio/cpp_files/CMakeFiles/eventslib.dir/depend:
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files/CMakeFiles/eventslib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : factorio/cpp_files/CMakeFiles/eventslib.dir/depend
