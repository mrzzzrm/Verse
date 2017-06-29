#include "VoxelObject.h"

#include "VoxelImpactSystem.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelObjectModification.h"
#include "VoxelWorld.h"

const VoxelObjectID & VoxelObject::id() const
{
    return m_id;
}

const Pose3D & VoxelObject::pose() const
{
    return m_pose;
};

float VoxelObject::scale() const
{
    return m_voxelData->scale();
}

void VoxelObject::setId(VoxelObjectID id)
{
    m_id = id;
}

void VoxelObject::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

void VoxelObject::setScale(float scale)
{
    m_voxelData->setScale(scale);
}

void VoxelObject::setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel)
{
    m_crucialVoxel = crucialVoxel;
    m_voxelData->setCrucialVoxel(crucialVoxel);
}

void VoxelObject::setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints)
{
    m_voxelData->setVoxelHealthPoints(voxel, healthPoints);
}

void VoxelObject::addVoxelsRaw(const std::vector<Voxel> & voxels)
{
    m_voxelData->addVoxelsRaw(voxels);

    VoxelObjectModification modification(shared_from_this());
    modification.additions = voxels;

    emit(modification);
}

void VoxelObject::removeVoxelsRaw(const std::vector<glm::uvec3> & voxels)
{
    m_voxelData->removeVoxelsRaw(voxels);

    VoxelObjectModification modification(shared_from_this());
    modification.removals = voxels;

    if (m_crucialVoxel)
    {
        auto crucialVoxel = *m_crucialVoxel;
        for (const auto & voxel : voxels)
        {
            if (voxel == crucialVoxel) voxelWorld().onCrucialVoxelDestroyed(*this);
        }
    }

    emit(modification);
}

std::vector<glm::uvec3> VoxelObject::processImpact(const glm::uvec3 & voxel, float intensity, float radius)
{
    if (m_invincible) return {};

    return VoxelImpactSystem().process(*this, voxel, intensity, radius);
}

void VoxelObject::schedule()
{
    m_voxelData->renderTree().schedule(m_pose);
}