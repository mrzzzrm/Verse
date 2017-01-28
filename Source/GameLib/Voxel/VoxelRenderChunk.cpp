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

VoxelRenderChunk::VoxelRenderChunk(const VoxelRenderChunk & other):
    m_cluster(other.m_cluster),
    m_voxelWorld(other.m_voxelWorld),
    m_marchingCubes(m_voxelWorld.marchingCubesTriangulation(), m_cluster, 1.0f),
    m_configCluster(other.m_configCluster),
    m_voxelCount(other.m_voxelCount),
    m_llfRender(other.m_llfRender),
    m_urbRender(other.m_urbRender),
    m_numVisibleVoxels(other.m_numVisibleVoxels),
    m_colorOverride(other.m_colorOverride)
{

}

void VoxelRenderChunk::addVoxel(const Voxel & voxel, bool visible)
{
    Assert(!m_cluster.test(voxel.cell), "Voxel already added");

    m_cluster.set(voxel.cell, voxel.color);

    m_llfDirty = glm::min(m_llfDirty, voxel.cell);
    m_urbDirty = glm::max(m_urbDirty, voxel.cell);

    if (visible)
    {
//        m_llfVisible = glm::min(m_llfVisible, voxel.cell);
//        m_urbVisible = glm::max(m_urbVisible, voxel.cell);
        m_numVisibleVoxels++;
    }

    m_drawDirty = true;
    m_voxelCount++;
}

void VoxelRenderChunk::removeVoxel(const glm::uvec3 & voxel, bool visible)
{
    Assert(m_cluster.test(voxel), "No voxel to remove");

    m_cluster.set(voxel, decltype(m_cluster)::EMPTY_VOXEL);

    m_llfDirty = glm::min(m_llfDirty, voxel);
    m_urbDirty = glm::max(m_urbDirty, voxel);

    if (visible)
    {
        Assert(m_numVisibleVoxels > 0, "");
        m_numVisibleVoxels--;
    }

    m_drawDirty = true;
    m_voxelCount--;
}

void VoxelRenderChunk::updateVoxelVisibility(const glm::uvec3 & voxel, bool visible)
{
    if (visible)
    {
        m_numVisibleVoxels++;
    }
    else
    {
        Assert(m_numVisibleVoxels > 0, "");
        m_numVisibleVoxels--;
    }
}

std::shared_ptr<VoxelRenderChunk> VoxelRenderChunk::clone()
{
    return std::make_shared<VoxelRenderChunk>(*this);
}

void VoxelRenderChunk::schedule(const Pose3D & pose) const
{
    if (m_voxelCount == 0) return;
    if (m_numVisibleVoxels == 0) return;

    if (m_drawDirty)
    {
        m_marchingCubes.onClusterChanged(m_llfDirty, m_urbDirty);
        //m_marchingCubes.run(glm::max(m_llfRender, m_llfVisible), glm::min(m_urbRender, m_urbVisible), m_colorOverride);
        m_marchingCubes.run(m_llfRender, m_urbRender, m_colorOverride);

        auto vertices = m_marchingCubes.takeVertices();

        m_meshEmpty = vertices.empty();

        if (!m_meshEmpty)
        {
            m_draw = m_voxelWorld.context().createDraw(m_voxelWorld.program());
            m_draw.addVertices(std::move(vertices));
            //     m_draw.state().setCullState(CullState::disabled());

            m_transformUniform = m_draw.uniform("Transform");
            m_viewProjectionUniform = m_draw.uniform("ViewProjection");

            m_drawDirty = false;
            m_llfDirty = glm::uvec3(std::numeric_limits<uint32_t>::max());
            m_urbDirty = glm::uvec3(0);
        }
    }

    if (m_meshEmpty) return;

    m_viewProjectionUniform.set(m_voxelWorld.camera().viewProjection());
    m_transformUniform.set(pose.matrix());
    m_draw.schedule();
}
