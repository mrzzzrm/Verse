set(target GameLib)

add_library(${target}
    STATIC

    Hailstorm/HailstormParticle.h
    Hailstorm/HailstormBullet.inl
    Hailstorm/HailstormDefines.h
    Hailstorm/HailstormManager.cpp
    Hailstorm/HailstormManager.h
    Hailstorm/HailstormPhysicsWorld.cpp
    Hailstorm/HailstormPhysicsWorld.h
    Hailstorm/HailstormRenderBatch.cpp
    Hailstorm/HailstormRenderBatch.h
    Hailstorm/HailstormRenderer.cpp
    Hailstorm/HailstormRenderer.h

    Npc/FlyToTask.cpp
    Npc/FlyToTask.h
    Npc/NpcController.cpp
    Npc/NpcController.h
    Npc/NpcFlightControl.cpp
    Npc/NpcFlightControl.h
    Npc/NpcTask.cpp
    Npc/NpcTask.h

    Vfx/Emitter.cpp
    Vfx/Emitter.h
    Vfx/EmitterIntensityStrategy.cpp
    Vfx/EmitterIntensityStrategy.h
    Vfx/EmitterLifetimeStrategy.cpp
    Vfx/EmitterLifetimeStrategy.h
    Vfx/EmitterPlacementStrategy.cpp
    Vfx/EmitterPlacementStrategy.h
    Vfx/EmitterVelocityStrategy.cpp
    Vfx/EmitterVelocityStrategy.h

    Voxel/VoxelCluster.cpp
    Voxel/VoxelCluster.h
    Voxel/VoxelCluster.inl
    Voxel/VoxelClusterContact.cpp
    Voxel/VoxelClusterContact.h
    Voxel/VoxelClusterMarchingCubes.cpp
    Voxel/VoxelClusterMarchingCubes.h
    Voxel/VoxelClusterMarchingCubesTriangulation.cpp
    Voxel/VoxelClusterMarchingCubesTriangulation.h
    Voxel/VoxelClusterPrimitiveTest.cpp
    Voxel/VoxelClusterPrimitiveTest.h
    Voxel/VoxelHull.cpp
    Voxel/VoxelHull.h
    Voxel/VoxelModel.cpp
    Voxel/VoxelModel.h
    Voxel/VoxelObject.cpp
    Voxel/VoxelObject.h
    Voxel/VoxelObjectVoxelData.cpp
    Voxel/VoxelObjectVoxelData.h
    Voxel/VoxelRenderChunk.cpp
    Voxel/VoxelRenderChunk.h
    Voxel/VoxelRenderChunkTree.cpp
    Voxel/VoxelRenderChunkTree.h
    Voxel/VoxelRigidBodyPayload.cpp
    Voxel/VoxelRigidBodyPayload.h
    Voxel/VoxelShape.cpp
    Voxel/VoxelShape.h
    Voxel/VoxelWorld.cpp
    Voxel/VoxelWorld.h
    Voxel/VoxReader.cpp
    Voxel/VoxReader.h

    AimHelper.cpp
    AimHelper.h
    CollisionShapeTypes.h
    Equipment.cpp
    Equipment.h
    FlightControlBase.cpp
    FlightControlBase.h
    Player/PlayerFlightControl.cpp
    Player/PlayerFlightControl.h
    FlightControlConfig.cpp
    FlightControlConfig.h
    Hardpoint.cpp
    Hardpoint.h
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Hailstorm
    ${CMAKE_CURRENT_SOURCE_DIR}/Npc
    ${CMAKE_CURRENT_SOURCE_DIR}/Vfx
    ${CMAKE_CURRENT_SOURCE_DIR}/Voxel

    ${DELIBERATION_INCLUDE_DIRS}
    )

target_link_libraries(${target}
    ${DELIBERATION_LIBRARIES}
    )