# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/ak/Gstreamer/MikeRecorder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ak/Gstreamer/MikeRecorder/build

# Include any dependencies generated for this target.
include CMakeFiles/MikeRecoderToFile.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MikeRecoderToFile.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MikeRecoderToFile.dir/flags.make

CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o: CMakeFiles/MikeRecoderToFile.dir/flags.make
CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o: ../GstMikeRecorder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ak/Gstreamer/MikeRecorder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o -c /home/ak/Gstreamer/MikeRecorder/GstMikeRecorder.cpp

CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ak/Gstreamer/MikeRecorder/GstMikeRecorder.cpp > CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.i

CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ak/Gstreamer/MikeRecorder/GstMikeRecorder.cpp -o CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.s

CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.requires:

.PHONY : CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.requires

CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.provides: CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.requires
	$(MAKE) -f CMakeFiles/MikeRecoderToFile.dir/build.make CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.provides.build
.PHONY : CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.provides

CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.provides.build: CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o


# Object files for target MikeRecoderToFile
MikeRecoderToFile_OBJECTS = \
"CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o"

# External object files for target MikeRecoderToFile
MikeRecoderToFile_EXTERNAL_OBJECTS =

../MikeRecoderToFile: CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o
../MikeRecoderToFile: CMakeFiles/MikeRecoderToFile.dir/build.make
../MikeRecoderToFile: CMakeFiles/MikeRecoderToFile.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ak/Gstreamer/MikeRecorder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../MikeRecoderToFile"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MikeRecoderToFile.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MikeRecoderToFile.dir/build: ../MikeRecoderToFile

.PHONY : CMakeFiles/MikeRecoderToFile.dir/build

CMakeFiles/MikeRecoderToFile.dir/requires: CMakeFiles/MikeRecoderToFile.dir/GstMikeRecorder.cpp.o.requires

.PHONY : CMakeFiles/MikeRecoderToFile.dir/requires

CMakeFiles/MikeRecoderToFile.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MikeRecoderToFile.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MikeRecoderToFile.dir/clean

CMakeFiles/MikeRecoderToFile.dir/depend:
	cd /home/ak/Gstreamer/MikeRecorder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ak/Gstreamer/MikeRecorder /home/ak/Gstreamer/MikeRecorder /home/ak/Gstreamer/MikeRecorder/build /home/ak/Gstreamer/MikeRecorder/build /home/ak/Gstreamer/MikeRecorder/build/CMakeFiles/MikeRecoderToFile.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MikeRecoderToFile.dir/depend
