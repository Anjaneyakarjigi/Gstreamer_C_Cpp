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
CMAKE_SOURCE_DIR = /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build

# Include any dependencies generated for this target.
include CMakeFiles/File2RTPStream.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/File2RTPStream.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/File2RTPStream.dir/flags.make

CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o: CMakeFiles/File2RTPStream.dir/flags.make
CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o: ../Stream_Video2RTP_Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o -c /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/Stream_Video2RTP_Server.cpp

CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/Stream_Video2RTP_Server.cpp > CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.i

CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/Stream_Video2RTP_Server.cpp -o CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.s

CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.requires:

.PHONY : CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.requires

CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.provides: CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.requires
	$(MAKE) -f CMakeFiles/File2RTPStream.dir/build.make CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.provides.build
.PHONY : CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.provides

CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.provides.build: CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o


# Object files for target File2RTPStream
File2RTPStream_OBJECTS = \
"CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o"

# External object files for target File2RTPStream
File2RTPStream_EXTERNAL_OBJECTS =

../File2RTPStream: CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o
../File2RTPStream: CMakeFiles/File2RTPStream.dir/build.make
../File2RTPStream: CMakeFiles/File2RTPStream.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../File2RTPStream"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/File2RTPStream.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/File2RTPStream.dir/build: ../File2RTPStream

.PHONY : CMakeFiles/File2RTPStream.dir/build

CMakeFiles/File2RTPStream.dir/requires: CMakeFiles/File2RTPStream.dir/Stream_Video2RTP_Server.cpp.o.requires

.PHONY : CMakeFiles/File2RTPStream.dir/requires

CMakeFiles/File2RTPStream.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/File2RTPStream.dir/cmake_clean.cmake
.PHONY : CMakeFiles/File2RTPStream.dir/clean

CMakeFiles/File2RTPStream.dir/depend:
	cd /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build /home/ak/Gstreamer/VideoFile2RTPServerStream/Linux/build/CMakeFiles/File2RTPStream.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/File2RTPStream.dir/depend

