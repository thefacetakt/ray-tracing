cmake_minimum_required (VERSION 2.6)
project (RayTracer)
add_definitions(`libpng-config --cflags`)
add_executable(RayTracer smth.cpp)

add_definitions(-std=c++11)
add_definitions(-O2)    

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(RayTracer ${PNG_LIBRARY})
