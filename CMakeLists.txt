cmake_minimum_required(VERSION 2.8)

project(tracker)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wextra -pedantic -Wno-long-long")
endif()

# Specify OpenCV directory (if needed, ensure correct path)
# Replace the path with your correct OpenCV path if needed
set(OpenCV_DIR "/opt/homebrew/Cellar/opencv/4.11.0/lib/cmake/opencv4")

# Find OpenCV package
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_LIB_DIR: ${OpenCV_LIB_DIR}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src/)

# Add subdirectory for source code
add_subdirectory(src)

# Add executables and link libraries
add_executable(asms_vot main_vot.cpp)
target_link_libraries(asms_vot ${OpenCV_LIBS} color)

add_executable(asms_demo main_demo.cpp)
target_link_libraries(asms_demo ${OpenCV_LIBS} color)

