#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelClusterShape.h"
#include "VoxelDefines.h"
#include "VoxelModel.h"

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

class VoxelObject final
{
public:
    VoxelObject(const std::shared_ptr<VoxelObjectPrototype> & prototype);
    ~VoxelObject();

    const VoxelObjectID & id() const;

    const Pose3D & pose() const;

    std::shared_ptr<RigidBody> & body();
    const std::shared_ptr<VoxelObjectPrototype> & prototype() const;

    void setId(VoxelObjectID id);

    void setPose(const Pose3D & pose);

    void schedule(const Camera3D & camera);

private:
    std::shared_ptr<VoxelObjectPrototype>
                    m_prototype;
    std::shared_ptr<VoxelRigidBodyPayload>
                    m_rigidBodyPayload;
    VoxelObjectID   m_id;
    Pose3D          m_pose;
    std::shared_ptr<RigidBody>
                    m_body;
};