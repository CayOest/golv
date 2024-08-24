cmake_minimum_required(VERSION 3.22)

project(golv)

if(LINUX)
    add_compile_options(-fconcepts-diagnostics-depth=3 -Wextra -Wpedantic -Wall)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(golv)
add_subdirectory(tests)

# add_subdirectory(examples)