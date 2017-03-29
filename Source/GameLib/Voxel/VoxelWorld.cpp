#include "VoxelWorld.h"

#include <Deliberation/Core/ScopeProfiler.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Scene/Camera3D.h>

VoxelWorld::VoxelWorld(World & world, const Camera3D & camera, const Texture & envMap):
    m_context(world.system<ApplicationSystem>().context()),
    m_physicsWorld(world.system<PhysicsWorldSystem>().physicsWorld()),
    m_camera(camera),
    m_envMap(envMap),
    Base(world, ComponentFilter::requires<VoxelObject>())
{
    m_program = m_context.createProgram({GameDataPath("Data/Shaders/Voxel.vert"),
                                         GameDataPath("Data/Shaders/Voxel.frag")});
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

void VoxelWorld::onEntityAdded(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();
    addVoxelObject(voxelObject.shared_from_this());
}

void VoxelWorld::onEntityRemoved(Entity & entity)
{

}

void VoxelWorld::onRender()
{
    for (auto & object : m_objects) object->schedule();
}

void VoxelWorld::onEntityUpdate(Entity & entity, float seconds)
{
    auto & voxelObject = entity.component<VoxelObject>();
    if (voxelObject.data().numVoxels() == 0)
    {
        entity.scheduleRemoval();
    }

    if (entity.hasComponent<RigidBodyComponent>())
    {
        auto & body = entity.component<RigidBodyComponent>().value();
        auto & object = entity.component<VoxelObject>();

        Pose3D pose;
        pose.setOrientation(body->transform().orientation());
        pose.setPosition(body->transform().position());
        pose.setCenter(body->transform().center());

        object.setPose(pose);
    }
}