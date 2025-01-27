project(benchmark)

add_executable(bm_test 
bm_bridge.cpp
)

add_executable(bm_skat 
bm_skat.cpp
)

target_include_directories(bm_test PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(bm_skat PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(bm_test 
golv)

target_link_libraries(bm_skat
golv)