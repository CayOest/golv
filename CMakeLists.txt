cmake_minimum_required(VERSION 3.22)

project(golv)

add_compile_options(-Wall -Wextra -Wpedantic -fconcepts-diagnostics-depth=3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(golv)
add_subdirectory(tests)

# add_subdirectory(examples)