#include "VoxelObject.h"

#include "VoxelRigidBodyPayload.h"

VoxelObject::VoxelObject(const VoxelObjectVoxelData & voxelData):
    m_voxelWorld(voxelData.voxelWorld()),
    m_voxelData(voxelData)
{
}

VoxelObject::~VoxelObject()
{
}

const VoxelObjectID & VoxelObject::id() const
{
    return m_id;
}

const Pose3D & VoxelObject::pose() const
{
    return m_pose;
};

const VoxelObjectVoxelData & VoxelObject::data() const
{
    return m_voxelData;
}

std::shared_ptr<RigidBody> & VoxelObject::body()
{
    if (!m_body)
    {
        m_rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(shared_from_this());
        m_body = std::make_shared<RigidBody>(m_voxelData.shapeTree());
        m_body->setPayload(m_rigidBodyPayload);
        m_body->transform().setCenter(glm::vec3(m_voxelData.size()) / 2.0f);
    }

    return m_body;
}

void VoxelObject::setId(VoxelObjectID id)
{
    m_id = id;
}

void VoxelObject::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

void VoxelObject::addVoxels(const std::vector<Voxel> & voxels)
{
    m_voxelData.addVoxels(voxels);
}

void VoxelObject::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    m_voxelData.removeVoxels(voxels);
}

void VoxelObject::schedule()
{
    m_voxelData.renderTree().schedule(m_pose);
}