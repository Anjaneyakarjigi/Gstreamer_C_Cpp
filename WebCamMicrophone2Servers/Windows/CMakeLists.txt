#Love Guru : 40809911
cmake_minimum_required(VERSION 2.8)

set(ProjectName WebCamMicrophone2Servers)
project(${ProjectName})

find_package(PkgConfig)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

include_directories(${PROJECT_SOURCE_DIR})

set(File2Compile call_server.cpp)
#set(${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(
        ${GLIB_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
)

#linking GStreamer library directory
link_directories(
        ${GLIB_LIBRARY_DIRS}
        ${GSTREAMER_LIBRARY_DIRS}
)

add_executable(${ProjectName} ${File2Compile})

target_link_libraries(${ProjectName} ${GSTREAMER_LIBRARIES})


#set(COPYEXE ${PROJECT_SOURCE_DIR}/soumiii.mp4)
#file(COPY ${COPYEXE} DESTINATION ${PROJECT_SOURCE_DIR}/Debug/)

#cmake -H. -Bbuild -G "Visual Studio 14 2015 Win64" --config Release
#cmake --build build --config Release
