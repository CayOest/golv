project(unit)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  unit
  games/_tictactoe.cpp
  games/_connectfour.cpp
  games/_bridge.cpp
  games/_skat.cpp
  games/_rps.cpp
  games/_kuhn.cpp
  algorithm/_alphabeta.cpp
  algorithm/_negamax.cpp
  algorithm/_mtd_f.cpp
  algorithm/_cfr.cpp
  util/_cyclic_number.cpp
)

target_include_directories(unit PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(
  unit
  GTest::gtest_main
  golv
)

include(GoogleTest)
gtest_discover_tests(unit)