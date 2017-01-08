set(target Tests)

include_directories(
    ${target}
    PRIVATE

    ${PROJECT_SOURCE_DIR}/Deliberation2/Extern/googletest/googletest/include/
)

add_executable(
    ${target}

    Tests.cpp
    VoxelClusterShapeTest.cpp
)

target_link_libraries(${target}
    GameLib
    gtest
    )