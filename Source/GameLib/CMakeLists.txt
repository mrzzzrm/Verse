set(target GameLib)

add_library(${target}
    STATIC

    GameLib.cpp
    GameLib.h
    )

target_include_directories(${target}
    PUBLIC

    ${DELIBERATION_INCLUDE_DIRS}
    )

target_link_libraries(${target}
    ${DELIBERATION_LIBRARIES}
    )