#pragma once

#include <Deliberation/Draw/Draw.h>

#include "GameLib.h"
#include "VoxelCluster.h"

namespace deliberation
{

class Camera3D;
class Pose3D;

}

class VoxelWorld;

class VoxelModel final
{
public:
    VoxelModel(VoxelWorld & world, const VoxelCluster<glm::vec3> & cluster);

    void schedule(const Camera3D & camera, const Pose3D & pose);

private:
    Draw m_draw;
};