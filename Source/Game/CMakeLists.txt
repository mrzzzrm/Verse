set(target Verse)

add_executable(
    ${target}

    Game.cpp
    )

target_link_libraries(${target}
    GameLib
    )