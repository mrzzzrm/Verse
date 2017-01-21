#include "VoxelObject.h"

#include "VoxelRigidBodyPayload.h"

VoxelObject::VoxelObject(const VoxelObjectVoxelData & voxelData):
    m_voxelWorld(voxelData.voxelWorld()),
    m_voxelData(voxelData)
{
//    m_prototype->incRefCount();
//
//    m_rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(m_id);
//    m_body = std::make_shared<RigidBody>(m_prototype->shape());
//    m_body->setPayload(m_rigidBodyPayload);
//    m_body->transform().setCenter(glm::vec3(prototype->cluster().size()) / 2.0f);
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
}

std::shared_ptr<RigidBody> & VoxelObject::body()
{
    return m_body;
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