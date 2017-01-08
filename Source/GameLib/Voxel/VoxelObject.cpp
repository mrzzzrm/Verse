#include "VoxelObject.h"

VoxelObjectID & VoxelObject::id()
{
    return m_id;
}

const VoxelObjectID & VoxelObject::id() const
{
    return m_id;
}

Pose3D & VoxelObject::pose()
{
    return m_pose;
}

const Pose3D & VoxelObject::pose() const
{
    return m_pose;
}

std::shared_ptr<const RigidBody> VoxelObject::body() const
{
    return m_body;
}

std::shared_ptr<const VoxelModel> VoxelObject::model() const
{
    return m_model;
}

void VoxelObject::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

void VoxelObject::setBody(const std::shared_ptr<RigidBody> & body)
{
    m_body = body;
}

void VoxelObject::setModel(const std::shared_ptr<VoxelModel> & model)
{
    m_model = model;
}
