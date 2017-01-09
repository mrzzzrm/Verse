#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Draw/Program.h>

#include "VoxelObject.h"
#include "VoxelClusterMarchingCubesTriangulation.h"

namespace deliberation
{

class Camera3D;
class Context;
class PhysicsWorld;

}

class VoxelObject;
class VoxelObjectID;

class VoxelWorld final
{
public:
    VoxelWorld(Context & context, PhysicsWorld & physicsWorld, const Camera3D & camera);

    Context & context() const;
    const VoxelClusterMarchingCubesTriangulation & marchingCubesTriangulation() const;
    const Program & program() const;

    void addVoxelObject(std::shared_ptr<VoxelObject> voxelObject);

    void removeVoxel(const VoxelObjectID & voxelObjectID, const glm::uvec3 & voxel);

    void update(float seconds);

private:
    Context &           m_context;
    PhysicsWorld &      m_physicsWorld;
    const Camera3D &    m_camera;
    VoxelClusterMarchingCubesTriangulation
                        m_marchingCubesTriangulation;
    Program             m_program;

    std::vector<std::shared_ptr<VoxelObject>>
                        m_objects;
};