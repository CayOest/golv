project(golv)

add_library(
    golv
    games/cards.cpp
    games/tictactoe.cpp
    games/connectfour.cpp
    games/bridge.cpp 
    util/logging.cpp
)

target_include_directories(golv PRIVATE ${CMAKE_SOURCE_DIR})
