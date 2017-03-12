#include "VoxelWorld.h"

#include <Deliberation/Core/ScopeProfiler.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Physics/PhysicsWorld.h>

VoxelWorld::VoxelWorld(World & world, Context & context, PhysicsWorld & physicsWorld, const Camera3D & camera, const Texture & envMap):
    m_context(context),
    m_physicsWorld(physicsWorld),
    m_camera(camera),
    m_envMap(envMap),
    Base(world)
{
    m_program = m_context.createProgram({"Data/Shaders/Voxel.vert",
                                         "Data/Shaders/Voxel.frag"});
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

const Texture & VoxelWorld::envMap() const
{
    return m_envMap;
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

void VoxelWorld::onRender()
{
    for (auto & object : m_objects) object->schedule();
}