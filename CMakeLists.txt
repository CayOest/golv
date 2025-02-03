cmake_minimum_required(VERSION 3.22)

project(golv)

cmake_policy(SET CMP0135 NEW)

if(LINUX)
    if(CMAKE_COMPILER_IS_GNUCXX)
        add_compile_options(-fconcepts-diagnostics-depth=3)
    endif()

    add_compile_options(-Wextra -Wall)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(golv)
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(python)