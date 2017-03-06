#include "VoxelObject.h"

#include "VoxelRigidBodyPayload.h"
#include "VoxelObjectModification.h"

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

void VoxelObject::setId(VoxelObjectID id)
{
    m_id = id;
}

void VoxelObject::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

void VoxelObject::setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints)
{
    m_voxelData.setVoxelHealthPoints(voxel, healthPoints);
}

void VoxelObject::addVoxels(const std::vector<Voxel> & voxels)
{
    m_voxelData.addVoxels(voxels);

    VoxelObjectModification modification(shared_from_this());
    modification.additions = voxels;

    emit(modification);
}

void VoxelObject::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    m_voxelData.removeVoxels(voxels);

    VoxelObjectModification modification(shared_from_this());
    modification.removals = voxels;

    emit(modification);
}

void VoxelObject::schedule()
{
    m_voxelData.renderTree().schedule(m_pose);
}