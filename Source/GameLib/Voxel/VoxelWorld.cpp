#include "VoxelWorld.h"

#include <algorithm>

#include <Deliberation/Core/ScopeProfiler.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include "VoxelClusterSplitSystem.h"
#include "VoxelObjectModification.h"

VoxelWorld::VoxelWorld(World & world, const Texture & envMap):
    Base(world, ComponentFilter::requires<Transform3DComponent, VoxelObject>()),
    m_drawContext(world.systemRef<ApplicationSystem>().drawContext()),
    m_envMap(envMap)
{
    m_renderer = world.systemRef<RenderSystem>().renderManager().addRenderer<VoxelRenderer>(envMap);

    m_program = m_drawContext.createProgram({GameDataPath("Data/Shaders/Voxel.vert"),
                                         GameDataPath("Data/Shaders/Voxel.frag")});
}

DrawContext & VoxelWorld::drawContext() const
{
    return m_drawContext;
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

void VoxelWorld::onCrucialVoxelDestroyed(VoxelObject & voxelObject)
{
    auto originalEntity = world().entityByIndex(voxelObject.entityIndex());
    const auto & originalRigidBody = originalEntity.component<RigidBodyComponent>().value();

    std::cout << "Turning '" << originalEntity.name()
              << "' to remnant because its crucial voxel was destroyed" << std::endl;

    auto remnantVoxelData = voxelObject.data();
    remnantVoxelData->setCrucialVoxel({});

    auto remnant = world().createEntity("Remnant of '" + originalEntity.name() + "'");
    auto & remnantVoxelObject = remnant.addComponent<VoxelObject>();
    remnantVoxelObject.setVoxelData(remnantVoxelData);

    auto remnantRigidBody = std::make_shared<RigidBody>(voxelObject.data()->shape());
    remnantRigidBody->setTransform(originalRigidBody->transform());
    remnant.addComponent<RigidBodyComponent>(remnantRigidBody);

    originalEntity.scheduleRemoval();
}

void VoxelWorld::onEntityAdded(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();
    auto & transform = entity.component<Transform3DComponent>().value();

    m_renderer->addVoxelObject(voxelObject.shared_from_this());
}

void VoxelWorld::onEntityRemoved(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();
    m_renderer->removeVoxelObject(voxelObject.shared_from_this());
}

void VoxelWorld::onEntityGameUpdate(Entity & entity, float seconds)
{
    auto & voxelObject = entity.component<VoxelObject>();
    if (voxelObject.data()->numVoxels() == 0)
    {
        entity.scheduleRemoval();
    }

    if (entity.hasComponent<Transform3DComponent>())
    {
        auto & transform = entity.component<Transform3DComponent>().value();
        auto & object = entity.component<VoxelObject>();

        object.setTransform(transform);
    }
}

void VoxelWorld::onGameUpdate(float seconds)
{

}