if (ENABLE_PYBIND)
    project(golv_skat)
    cmake_minimum_required(VERSION 3.14)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.11.1  # Falls du eine bestimmte Version willst
    )
    FetchContent_MakeAvailable(pybind11)

    add_library(golv_skat MODULE golv_skat.cpp)

    target_include_directories(golv_skat PRIVATE ${CMAKE_SOURCE_DIR})

    target_link_libraries(golv_skat PRIVATE pybind11::module golv)
endif()