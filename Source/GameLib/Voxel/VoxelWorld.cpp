#include "VoxelWorld.h"

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
    m_objects.emplace_back(voxelObject);
    m_physicsWorld.addRigidBody(voxelObject->body());
}

void VoxelWorld::removeVoxel(const VoxelObjectID & voxelObjectID, const glm::uvec3 & voxel)
{

}

void VoxelWorld::update(float seconds)
{
    for (auto & object : m_objects)
    {
        Pose3D pose;

        pose.setPosition(object->body()->transform().position());
        pose.setOrientation(object->body()->transform().orientation());
        pose.setCenter(object->body()->transform().center());

        object->setPose(pose);

        object->schedule(m_camera);
    }
}