#pragma once

#include <memory>

#include <boost/optional.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Component.h>

#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelDefines.h"
#include "VoxelObjectModification.h"
#include "VoxelObjectVoxelData.h"

namespace deliberation
{
class Camera3D;
}

class VoxelObjectPrototype;

struct VoxelObjectID
{
    VoxelObjectWorldUID worldUID = INVALID_VOXEL_OBJECT_WORLD_UID;
};

class VoxelObject final : public Component<VoxelObject>
{
    DELIBERATION_COMPONENT_NAME("VoxelObject")
public:
    VoxelWorld & voxelWorld() const
    {
        AssertM((bool)m_voxelData, "");
        return m_voxelData->voxelWorld();
    }
    const VoxelObjectID & id() const;
    const Transform3D &   transform() const { return m_transform; }
    const std::shared_ptr<VoxelObjectVoxelData> & data() const
    {
        return m_voxelData;
    }

    void setVoxelData(const std::shared_ptr<VoxelObjectVoxelData> & voxelData);
    void setId(VoxelObjectID id);
    void setTransform(const Transform3D & transform)
    {
        m_transform = transform;
    }
    void setInvincible(bool invincible) { m_invincible = invincible; }
    void setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel);

    void setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints);

    // @{
    /**
     * Perform no checks whether the voxels already exist or whether the cells
     * are empty
     */
    void addVoxelsRaw(const std::vector<Voxel> & voxels);
    void removeVoxelsRaw(
        const std::vector<glm::uvec3> & voxels);
    // @}

    void processImpact(const glm::uvec3 & voxel, float intensity, float radius);
    void performSplitDetection()
    {
        m_voxelData->splitDetector().performSplitDetection();
    }

    void render();

private:
    void destroyVoxels(std::vector<glm::uvec3> voxels);
    void checkCrucialVoxelForRemoval(const std::vector<glm::uvec3> & voxels);

private:
    std::shared_ptr<VoxelObjectVoxelData> m_voxelData;
    VoxelObjectID                         m_id;
    Transform3D                           m_transform;
    bool                                  m_invincible = false;
    boost::optional<glm::uvec3>           m_crucialVoxel;
};