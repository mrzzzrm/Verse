set(target GameLib)

add_library(${target}
    STATIC

    Hailstorm/HailstormBullet.h
    Hailstorm/HailstormBullet.inl
    Hailstorm/HailstormManager.cpp
    Hailstorm/HailstormManager.h
    Hailstorm/HailstormPhysicsWorld.cpp
    Hailstorm/HailstormPhysicsWorld.h
    Hailstorm/HailstormRenderBatch.cpp
    Hailstorm/HailstormRenderBatch.h
    Hailstorm/HailstormRenderer.cpp
    Hailstorm/HailstormRenderer.h

    Voxel/VoxelCluster.cpp
    Voxel/VoxelCluster.h
    Voxel/VoxelCluster.inl
    Voxel/VoxelClusterContact.cpp
    Voxel/VoxelClusterContact.h
    Voxel/VoxelClusterMarchingCubes.cpp
    Voxel/VoxelClusterMarchingCubes.h
    Voxel/VoxelClusterMarchingCubesTriangulation.cpp
    Voxel/VoxelClusterMarchingCubesTriangulation.h
    Voxel/VoxelClusterShape.cpp
    Voxel/VoxelClusterShape.h
    Voxel/VoxelClusterShape.inl
    Voxel/VoxReader.cpp
    Voxel/VoxReader.h

    CollisionShapeTypes.h
    FlightControl.cpp
    FlightControl.h
    FlightControlConfig.h
    GameLib.cpp
    GameLib.h
    PlayerInput.cpp
    PlayerInput.h
    Weapon.cpp
    Weapon.h
    )

target_include_directories(${target}
    PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}

    ${DELIBERATION_INCLUDE_DIRS}
    )

target_link_libraries(${target}
    ${DELIBERATION_LIBRARIES}
    )