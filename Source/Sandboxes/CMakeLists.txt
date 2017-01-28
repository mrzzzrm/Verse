set(target VoxelObjectSandbox)

add_executable(
    ${target}

    VoxelObjectSandbox.cpp
    )

target_link_libraries(${target}
    GameLib
    )