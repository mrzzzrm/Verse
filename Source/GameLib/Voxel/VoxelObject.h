#pragma once

#include <memory>

#include <boost/optional.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Component.h>

#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelDefines.h"
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

class VoxelObject final:
    public Component<VoxelObject>
{
public:
    VoxelObject(const VoxelObjectVoxelData & voxelData);

    VoxelWorld & voxelWorld() const { return m_voxelWorld; }
    const VoxelObjectID & id() const;
    const Pose3D & pose() const;
    const VoxelObjectVoxelData & data() const;
    float scale() const;

    void setId(VoxelObjectID id);
    void setPose(const Pose3D & pose);
    void setScale(float scale);
    void setInvincible(bool invincible) { m_invincible = invincible; }
    void setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel);

    void setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints);

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    std::vector<glm::uvec3> processImpact(const glm::uvec3 & voxel, float intensity, float radius);
    void performSplitDetection() { m_voxelData.splitDetector().performSplitDetection(); }

    void schedule();

private:
    VoxelWorld &                m_voxelWorld;
    VoxelObjectVoxelData        m_voxelData;
    VoxelObjectID               m_id;
    Pose3D                      m_pose;
    std::shared_ptr<RigidBody>  m_body;
    bool                        m_invincible = false;
    boost::optional<glm::uvec3> m_crucialVoxel;
};