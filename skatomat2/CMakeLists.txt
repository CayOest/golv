project(skatomat2)

find_package(Boost REQUIRED)

if(Boost_FOUND)
    add_library(
        skatomat2
        game.cpp
        player.cpp
        rules.cpp
        skatexception.cpp
        state.cpp
        trick.cpp
        types.cpp
        util.cpp
    )

    target_include_directories(skatomat2 PRIVATE ${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
    target_link_libraries(skatomat2 PRIVATE ${Boost_LIBRARIES})
endif()
