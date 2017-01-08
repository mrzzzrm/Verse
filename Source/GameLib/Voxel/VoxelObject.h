#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelClusterShape.h"
#include "VoxelModel.h"

struct VoxelObjectID
{
    size_t worldIndex = 0;
};

class  VoxelObject
{
public:
    VoxelObjectID & id();
    const VoxelObjectID & id() const;

    Pose3D & pose();
    const Pose3D & pose() const;

    std::shared_ptr<const RigidBody> body() const;
    std::shared_ptr<const VoxelModel> model() const;

    void setPose(const Pose3D & pose);
    void setBody(const std::shared_ptr<RigidBody> & body);
    void setModel(const std::shared_ptr<VoxelModel> & model);

private:
    VoxelObjectID   m_id;
    Pose3D          m_pose;
    std::shared_ptr<RigidBody>
                    m_body;
    std::shared_ptr<VoxelModel>
                    m_model;
};