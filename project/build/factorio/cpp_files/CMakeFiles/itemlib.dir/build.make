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
include factorio/cpp_files/CMakeFiles/itemlib.dir/depend.make

# Include the progress variables for this target.
include factorio/cpp_files/CMakeFiles/itemlib.dir/progress.make

# Include the compile flags for this target's objects.
include factorio/cpp_files/CMakeFiles/itemlib.dir/flags.make

factorio/cpp_files/CMakeFiles/itemlib.dir/item.cpp.o: factorio/cpp_files/CMakeFiles/itemlib.dir/flags.make
factorio/cpp_files/CMakeFiles/itemlib.dir/item.cpp.o: ../factorio/cpp_files/item.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object factorio/cpp_files/CMakeFiles/itemlib.dir/item.cpp.o"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/itemlib.dir/item.cpp.o -c /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files/item.cpp

factorio/cpp_files/CMakeFiles/itemlib.dir/item.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/itemlib.dir/item.cpp.i"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files/item.cpp > CMakeFiles/itemlib.dir/item.cpp.i

factorio/cpp_files/CMakeFiles/itemlib.dir/item.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/itemlib.dir/item.cpp.s"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files/item.cpp -o CMakeFiles/itemlib.dir/item.cpp.s

# Object files for target itemlib
itemlib_OBJECTS = \
"CMakeFiles/itemlib.dir/item.cpp.o"

# External object files for target itemlib
itemlib_EXTERNAL_OBJECTS =

factorio/cpp_files/libitemlib.a: factorio/cpp_files/CMakeFiles/itemlib.dir/item.cpp.o
factorio/cpp_files/libitemlib.a: factorio/cpp_files/CMakeFiles/itemlib.dir/build.make
factorio/cpp_files/libitemlib.a: factorio/cpp_files/CMakeFiles/itemlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libitemlib.a"
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && $(CMAKE_COMMAND) -P CMakeFiles/itemlib.dir/cmake_clean_target.cmake
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/itemlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
factorio/cpp_files/CMakeFiles/itemlib.dir/build: factorio/cpp_files/libitemlib.a

.PHONY : factorio/cpp_files/CMakeFiles/itemlib.dir/build

factorio/cpp_files/CMakeFiles/itemlib.dir/clean:
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files && $(CMAKE_COMMAND) -P CMakeFiles/itemlib.dir/cmake_clean.cmake
.PHONY : factorio/cpp_files/CMakeFiles/itemlib.dir/clean

factorio/cpp_files/CMakeFiles/itemlib.dir/depend:
	cd /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/factorio/cpp_files /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files /mnt/d/studium/vorlesungen/advanced_programming_techniques/AdvPT/project/build/factorio/cpp_files/CMakeFiles/itemlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : factorio/cpp_files/CMakeFiles/itemlib.dir/depend

