project(examples)

add_executable(skat_solver
skat/skat_solver.cpp
)

add_executable(skat_pusher
skat/skat_push_optimizer.cpp
)

target_include_directories(skat_solver PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(skat_solver 
golv)

target_include_directories(skat_pusher PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(skat_pusher 
golv)