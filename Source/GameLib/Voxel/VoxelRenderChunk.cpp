#include "VoxelRenderChunk.h"

#include <bitset>

#include <glm/gtx/extented_min_max.hpp>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>

#include "VoxelClusterMarchingCubes.h"
#include "VoxelWorld.h"

VoxelRenderChunk::VoxelRenderChunk(const VoxelWorld & voxelWorld, const glm::uvec3 & size,
                                   const glm::uvec3 & llfRender, const glm::uvec3 & urbRender,
                                   const Optional<glm::vec3> & colorOverride):
    m_cluster(size),
    m_voxelWorld(voxelWorld),
    m_marchingCubes(voxelWorld.marchingCubesTriangulation(), m_cluster, 1.0f),
    m_configCluster(size - glm::uvec3(1)),
    m_llfRender(llfRender),
    m_urbRender(urbRender),
    m_colorOverride(colorOverride)
{
//    m_llfVisible = glm::uvec3(std::numeric_limits<uint32_t>::max());
//    m_urbVisible = glm::uvec3(0);
}

void VoxelRenderChunk::addVoxel(const Voxel & voxel)
{
    Assert(!m_cluster.test(voxel.cell), "Voxel already added");

    m_cluster.set(voxel.cell, voxel.color);

    m_llfDirty = glm::min(m_llfDirty, voxel.cell);
    m_urbDirty = glm::max(m_urbDirty, voxel.cell);

    if (voxel.hull)
    {
//        m_llfVisible = glm::min(m_llfVisible, voxel.cell);
//        m_urbVisible = glm::max(m_urbVisible, voxel.cell);
        m_hullVoxelCount++;
    }

    m_drawDirty = true;
    m_voxelCount++;
}

void VoxelRenderChunk::removeVoxel(const glm::uvec3 & voxel)
{
    Assert(m_cluster.test(voxel), "No voxel to remove");

    m_cluster.set(voxel, decltype(m_cluster)::EMPTY_VOXEL);

    m_llfDirty = glm::min(m_llfDirty, voxel);
    m_urbDirty = glm::max(m_urbDirty, voxel);

    m_drawDirty = true;
    m_voxelCount--;
}

std::shared_ptr<VoxelRenderChunk> VoxelRenderChunk::clone()
{

}

void VoxelRenderChunk::schedule(const Pose3D & pose) const
{
    if (m_voxelCount == 0) return;
    //if (m_hullVoxelCount == 0) return;

    if (m_drawDirty)
    {
        m_marchingCubes.onClusterChanged(m_llfDirty, m_urbDirty);
        //m_marchingCubes.run(glm::max(m_llfRender, m_llfVisible), glm::min(m_urbRender, m_urbVisible), m_colorOverride);
        m_marchingCubes.run(m_llfRender, m_urbRender, m_colorOverride);

        m_draw = m_voxelWorld.context().createDraw(m_voxelWorld.program());
        m_draw.addVertices(m_marchingCubes.takeVertices());
        m_draw.state().setCullState(CullState::disabled());

        m_transformUniform = m_draw.uniform("Transform");
        m_viewProjectionUniform = m_draw.uniform("ViewProjection");

        m_drawDirty = false;
        m_llfDirty = glm::uvec3(std::numeric_limits<uint32_t>::max());
        m_urbDirty = glm::uvec3(0);
    }

    m_viewProjectionUniform.set(m_voxelWorld.camera().viewProjection());
    m_transformUniform.set(pose.matrix());
    m_draw.schedule();
}
