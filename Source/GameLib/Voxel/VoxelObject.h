#pragma once

#include <memory>

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
class VoxelRigidBodyPayload;

struct VoxelObjectID
{
    VoxelObjectWorldUID worldUID = INVALID_VOXEL_OBJECT_WORLD_UID;
};

class VoxelObject final:
    public Component<VoxelObject>
{
public:
    VoxelObject(const VoxelObjectVoxelData & voxelData);
    ~VoxelObject();

    const VoxelObjectID & id() const;
    const Transform3D & transform() const;
    const VoxelObjectVoxelData & data() const;

    void setId(VoxelObjectID id);
    void setTransform(const Transform3D & transform);

    void setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints);

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    void schedule();

private:
    const VoxelWorld &      m_voxelWorld;
    VoxelObjectVoxelData    m_voxelData;
    std::shared_ptr<VoxelRigidBodyPayload>
                            m_rigidBodyPayload;
    VoxelObjectID           m_id;
    Transform3D             m_transform;
    std::shared_ptr<RigidBody>
                            m_body;
};