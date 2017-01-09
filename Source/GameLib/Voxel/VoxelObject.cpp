#include "VoxelObject.h"

#include "VoxelObjectPrototype.h"
#include "VoxelRigidBodyPayload.h"

VoxelObject::VoxelObject(const std::shared_ptr<VoxelObjectPrototype> & prototype):
    m_prototype(prototype)
{
    m_rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(m_id);
    m_body = std::make_shared<RigidBody>(m_prototype->shape());
    m_body->setPayload(m_rigidBodyPayload);
    m_body->transform().setCenter(glm::vec3(prototype->cluster().size()) / 2.0f);
}

VoxelObjectID & VoxelObject::id()
{
    return m_id;
}

const VoxelObjectID & VoxelObject::id() const
{
    return m_id;
}

const Pose3D & VoxelObject::pose() const
{
    return m_pose;
}

std::shared_ptr<RigidBody> & VoxelObject::body()
{
    return m_body;
}

const std::shared_ptr<VoxelObjectPrototype> & VoxelObject::prototype() const
{
    return m_prototype;
}

void VoxelObject::setId(VoxelObjectID id)
{
    m_id = id;
    m_rigidBodyPayload->voxelObjectID = id;
}

void VoxelObject::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

void VoxelObject::schedule(const Camera3D & camera)
{
    m_prototype->model().schedule(camera, m_pose);
}