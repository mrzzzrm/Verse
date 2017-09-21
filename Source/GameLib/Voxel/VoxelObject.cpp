#include "VoxelObject.h"

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/ApplicationRuntime.h>

#include "VoxelImpactSystem.h"
#include "VoxelObjectModification.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelWorld.h"

const VoxelObjectID & VoxelObject::id() const { return m_id; }

void VoxelObject::setVoxelData(
    const std::shared_ptr<VoxelObjectVoxelData> & voxelData)
{
    m_voxelData = voxelData;
}

void VoxelObject::setId(VoxelObjectID id) { m_id = id; }

void VoxelObject::setCrucialVoxel(
    const boost::optional<glm::uvec3> & crucialVoxel)
{
    m_crucialVoxel = crucialVoxel;
    m_voxelData->setCrucialVoxel(crucialVoxel);
}

void VoxelObject::setVoxelHealthPoints(
    const glm::uvec3 & voxel, float healthPoints)
{
    m_voxelData->setVoxelHealthPoints(voxel, healthPoints);
}

void VoxelObject::addVoxelsRaw(const std::vector<Voxel> & voxels)
{
    m_voxelData->addVoxelsRaw(voxels);
}

void VoxelObject::removeVoxelsRaw(
    const std::vector<glm::uvec3> & voxels)
{
    m_voxelData->removeVoxelsRaw(voxels);
}

void VoxelObject::processImpact(
    const glm::uvec3 & voxel, float intensity, float radius)
{
    if (m_invincible) return;

    auto destroyedVoxels = VoxelImpactSystem().process(*this, voxel, intensity, radius);
    destroyVoxels(std::move(destroyedVoxels));
}

void VoxelObject::render() { m_voxelData->renderTree().render(m_transform); }

void VoxelObject::destroyVoxels(std::vector<glm::uvec3> voxels)
{
    if (voxels.empty()) return;

    removeVoxelsRaw(voxels);

    VoxelObjectModification modification(((World *)world())->entityById(entityId()));
    modification.destructions = std::move(voxels);

    checkCrucialVoxelForRemoval(modification.destructions);

    Application::get().runtime()->events()->publishEvent(modification);
}

void VoxelObject::checkCrucialVoxelForRemoval(const std::vector<glm::uvec3> & voxels)
{
    if (!m_crucialVoxel) return;

    auto crucialVoxel = *m_crucialVoxel;
    for (const auto & voxel : voxels)
    {
        if (voxel == crucialVoxel)
            voxelWorld().onCrucialVoxelDestroyed(*this);
    }
}