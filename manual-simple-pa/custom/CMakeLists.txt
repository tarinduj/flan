cmake_minimum_required(VERSION 3.10)

project(my_project)

# Abseil requires C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(simple_pa_manual.out simple_pa_manual.cpp)
add_executable(testHash.out testHash.cpp)

# mkdir build
# cd build 
# cmake ..
# cmake --build . --target simple_pa_manual.out