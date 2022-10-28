project(framework)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -fno-omit-frame-pointer")

add_executable(example1 ex1/main.cpp)
add_executable(gen ex1/gen.cpp)

add_executable(example2 ex2/main.cpp)

add_executable(example3 ex3/main.cpp)

add_executable(example4 ex4/main.cpp)
