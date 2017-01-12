#pragma once

#include <memory>

#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Physics/NarrowphasePrimitiveTest.h>
#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelObject.h"

struct RayCastVoxelClusterIntersection:
    RayCastIntersection
{
    RayCastVoxelClusterIntersection(const RigidBody & body);

    VoxelObjectID voxelObjectID;
    glm::uvec3    voxel;
};

class VoxelClusterPrimitiveTest:
    public NarrowphasePrimitiveTest
{
public:
    virtual std::unique_ptr<RayCastIntersection> lineTest(const Ray3D & ray,
                                                         const RigidBody & body) const override;
};