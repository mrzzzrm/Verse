set(target Tests)

include_directories(
    ${target}
    PRIVATE

    ${PROJECT_SOURCE_DIR}/Deliberation/Extern/googletest/googletest/include/
)

add_executable(
    ${target}

    Tests.cpp
    VoxelHullTest.cpp
    VoxelShapeTreeTest.cpp
)

target_link_libraries(${target}
    GameLib
    gtest
    )