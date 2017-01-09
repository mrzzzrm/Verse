#include "VoxelModel.h"

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>

#include "VoxelClusterMarchingCubes.h"
#include "VoxelWorld.h"

VoxelModel::VoxelModel(VoxelWorld & world, const VoxelCluster<glm::vec3> & cluster)
{
    auto marchingCubes = VoxelClusterMarchingCubes(world.marchingCubesTriangulation(), cluster, 1.0f);
    marchingCubes.run();

    m_draw = world.context().createDraw(world.program());
    m_draw.addVertices(marchingCubes.takeVertices());
    m_draw.state().setCullState(CullState::disabled());
}

void VoxelModel::schedule(const Camera3D & camera, const Pose3D & pose)
{
    m_draw.uniform("Transform").set(pose.matrix());
    m_draw.uniform("ViewProjection").set(camera.viewProjection());
    m_draw.schedule();
}