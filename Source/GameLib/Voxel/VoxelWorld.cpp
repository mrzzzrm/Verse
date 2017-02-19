#include "VoxelWorld.h"

#include <Deliberation/Core/ScopeProfiler.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Physics/PhysicsWorld.h>

VoxelWorld::VoxelWorld(Context & context, PhysicsWorld & physicsWorld, const Camera3D & camera):
    m_context(context),
    m_physicsWorld(physicsWorld),
    m_camera(camera)
{
    m_program = m_context.createProgram({deliberation::dataPath("Data/Shaders/VoxelExample.vert"),
                                         deliberation::dataPath("Data/Shaders/VoxelExample.frag")});
}

Context & VoxelWorld::context() const
{
    return m_context;
}

const Camera3D & VoxelWorld::camera() const
{
    return m_camera;
}

const VoxelClusterMarchingCubesTriangulation & VoxelWorld::marchingCubesTriangulation() const
{
    return m_marchingCubesTriangulation;
}

const Program & VoxelWorld::program() const
{
    return m_program;
}

void VoxelWorld::addVoxelObject(std::shared_ptr<VoxelObject> voxelObject)
{
    VoxelObjectID id;
    id.worldUID = m_uidIncrementor;

    voxelObject->setId(id);
    m_uidIncrementor++;

    m_objects.emplace_back(voxelObject);

    m_objectsByUID[id.worldUID] = voxelObject;
}

void VoxelWorld::render()
{
    for (auto & object : m_objects)
    {
        object->schedule();
    }
}