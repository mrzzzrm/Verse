add_library(SandboxCommon
    Common/SandboxApplication.cpp
    Common/SandboxApplication.h)
target_link_libraries(SandboxCommon GameLib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)

add_executable(VoxelObjectSandbox VoxelObjectSandbox.cpp)
target_link_libraries(VoxelObjectSandbox SandboxCommon)

add_executable(VfxSandbox VfxSandbox.cpp)
target_link_libraries(VfxSandbox SandboxCommon)

add_executable(NpcSandbox NpcSandbox.cpp)
target_link_libraries(NpcSandbox SandboxCommon)

add_executable(ResourceSandbox ResourceSandbox.cpp)
target_link_libraries(ResourceSandbox SandboxCommon)

add_executable(ShootingRangeSandbox ShootingRangeSandbox.cpp)
target_link_libraries(ShootingRangeSandbox SandboxCommon)

add_executable(HudSandbox HudSandbox.cpp)
target_link_libraries(HudSandbox SandboxCommon)

add_executable(VerseSandbox VerseSandbox.cpp)
target_link_libraries(VerseSandbox SandboxCommon)