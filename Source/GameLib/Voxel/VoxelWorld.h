#pragma once

#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include <Deliberation/Draw/Program.h>

#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelDefines.h"
#include "VoxelObject.h"

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
    const Camera3D & camera() const;
    const VoxelClusterMarchingCubesTriangulation & marchingCubesTriangulation() const;
    const Program & program() const;

    void addVoxelObject(std::shared_ptr<VoxelObject> voxelObject);

    void render();

private:
    Context &           m_context;
    PhysicsWorld &      m_physicsWorld;
    const Camera3D &    m_camera;
    VoxelClusterMarchingCubesTriangulation
                        m_marchingCubesTriangulation;
    Program             m_program;

    std::vector<std::shared_ptr<VoxelObject>>
                        m_objects;

    std::unordered_map<VoxelObjectWorldUID, std::shared_ptr<VoxelObject>>
                        m_objectsByUID;

    u64                 m_uidIncrementor = 0;
};