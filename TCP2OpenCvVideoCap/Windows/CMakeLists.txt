#Love Guru : 40809911
cmake_minimum_required(VERSION 2.8)

set(ProjectName TCP2OpenCvVideoCap)
project(${ProjectName})

include_directories(${PROJECT_SOURCE_DIR})

set(cvImageToGstreamerFile TCP2OpenCvVideoCap.cpp)
#set(${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LINK_DIRS})

add_executable(${ProjectName} ${cvImageToGstreamerFile})

target_link_libraries(${ProjectName} ${OpenCV_LIBS})

#cmake -H. -Bbuild -G "Visual Studio 14 2015 Win64" --config Release
#cmake --build build --config Release
