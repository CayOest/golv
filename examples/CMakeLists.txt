project(examples)

add_executable(skat_solver
skat_solver.cpp
)

target_include_directories(skat_solver PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(skat_solver 
golv)