project(benchmark)

add_executable(bm_test 
bm_bridge.cpp
)

target_include_directories(bm_test PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(bm_test 
golv)