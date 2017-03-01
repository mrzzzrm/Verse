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

VoxelObjectVoxelData & VoxelObject::data()
{
    return m_voxelData;
}

const VoxelObjectVoxelData & VoxelObject::data() const
{
    return m_voxelData;
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