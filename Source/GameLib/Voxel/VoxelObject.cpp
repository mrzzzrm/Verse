#include "VoxelObject.h"

#include "VoxelImpactSystem.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelObjectModification.h"
#include "VoxelWorld.h"

const VoxelObjectID & VoxelObject::id() const
{
    return m_id;
}

void VoxelObject::setVoxelData(const std::shared_ptr<VoxelObjectVoxelData> & voxelData)
{
    m_voxelData = voxelData;
}

void VoxelObject::setId(VoxelObjectID id)
{
    m_id = id;
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

    VoxelObjectModification modification(((World*)world())->entityById(entityId()));
    modification.additions = voxels;

    publishEvent(modification);
}

void VoxelObject::removeVoxelsRaw(const std::vector<glm::uvec3> & voxels, VoxelRemovalReason reason)
{
    m_voxelData->removeVoxelsRaw(voxels);

    VoxelObjectModification modification(((World*)world())->entityById(entityId()));
    if (reason == VoxelRemovalReason::Destruction) modification.destructions = voxels;
    else modification.splits = voxels;

    if (m_crucialVoxel)
    {
        auto crucialVoxel = *m_crucialVoxel;
        for (const auto & voxel : voxels)
        {
            if (voxel == crucialVoxel) voxelWorld().onCrucialVoxelDestroyed(*this);
        }
    }

    publishEvent(modification);
}

void VoxelObject::processImpact(const glm::uvec3 & voxel, float intensity, float radius)
{
    if (m_invincible) return;

    VoxelImpactSystem().process(*this, voxel, intensity, radius);
}

void VoxelObject::render()
{
    m_voxelData->renderTree().render(m_transform);
}