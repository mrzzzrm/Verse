#include "VoxelRenderChunk.h"

#include <bitset>

#include <glm/gtx/extented_min_max.hpp>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include "ColorPalette.h"
#include "VoxelCluster.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelRenderable.h"
#include "VoxelWorld.h"

VoxelRenderChunk::VoxelRenderChunk(
    VoxelRenderable &           renderable,
    const glm::uvec3 &          position,
    const glm::uvec3 &          size,
    const glm::uvec3 &          llfRender,
    const glm::uvec3 &          urbRender,
    const Optional<glm::vec3> & colorOverride)
    : m_cluster(size)
    , m_renderable(renderable)
    , m_marchingCubes(
          m_renderable.voxelWorld().marchingCubesTriangulation(),
          m_cluster,
          1.0f,
          position)
    , m_configCluster(size - glm::uvec3(1))
    , m_llfRender(llfRender)
    , m_urbRender(urbRender)
    , m_colorOverride(colorOverride)
{
    //    m_llfVisible = glm::uvec3(std::numeric_limits<uint32_t>::max());
    //    m_urbVisible = glm::uvec3(0);
}

VoxelRenderChunk::VoxelRenderChunk(const VoxelRenderChunk & other)
    : m_cluster(other.m_cluster)
    , m_renderable(other.m_renderable)
    , m_marchingCubes(
          m_renderable.voxelWorld().marchingCubesTriangulation(),
          m_cluster,
          1.0f,
          other.m_marchingCubes.offset())
    , m_configCluster(other.m_configCluster)
    , m_voxelCount(other.m_voxelCount)
    , m_meshEmpty(other.m_meshEmpty)
    , m_llfRender(other.m_llfRender)
    , m_urbRender(other.m_urbRender)
    , m_numVisibleVoxels(other.m_numVisibleVoxels)
    , m_colorOverride(other.m_colorOverride)
{
    /**
     * We don't deep-copy the marching cubes state, so set all of it to dirty
     */
    m_llfDirty = glm::uvec3(0, 0, 0);
    auto size = m_cluster.size();
    m_urbDirty = glm::uvec3(size - 1u);
}

void VoxelRenderChunk::addVoxel(const Voxel & voxel, bool visible)
{
    AssertM(!m_cluster.test(voxel.cell), "Voxel already added");

    m_cluster.set(voxel.cell, voxel.colorIndex);

    m_llfDirty = glm::min(m_llfDirty, voxel.cell);
    m_urbDirty = glm::max(m_urbDirty, voxel.cell);

    if (visible)
    {
        //        m_llfVisible = glm::min(m_llfVisible, voxel.cell);
        //        m_urbVisible = glm::max(m_urbVisible, voxel.cell);
        m_numVisibleVoxels++;
    }

    m_verticesDirty = true;
    m_voxelCount++;
}

void VoxelRenderChunk::removeVoxel(const glm::uvec3 & voxel, bool visible)
{
    AssertM(m_cluster.test(voxel), "No voxel to remove");

    m_cluster.set(voxel, decltype(m_cluster)::EMPTY_VOXEL);

    m_llfDirty = glm::min(m_llfDirty, voxel);
    m_urbDirty = glm::max(m_urbDirty, voxel);

    if (visible)
    {
        Assert(m_numVisibleVoxels > 0);
        m_numVisibleVoxels--;
    }

    m_verticesDirty = true;
    m_voxelCount--;
}

void VoxelRenderChunk::updateVoxelVisibility(
    const glm::uvec3 & voxel, bool visible)
{
    if (visible)
    {
        m_numVisibleVoxels++;
    }
    else
    {
        Assert(m_numVisibleVoxels > 0);
        m_numVisibleVoxels--;
    }
}

std::shared_ptr<VoxelRenderChunk> VoxelRenderChunk::clone()
{
    return std::make_shared<VoxelRenderChunk>(*this);
}

bool VoxelRenderChunk::updateVertices()
{
    if (m_voxelCount == 0 || m_numVisibleVoxels == 0)
    {
        if (m_verticesDirty)
        {
            if (!m_vertices.empty()) m_vertices.resize(0);
            return true;
        }
        return false;
    }

    if (m_verticesDirty)
    {
        m_marchingCubes.onClusterChanged(m_llfDirty, m_urbDirty);
        // m_marchingCubes.run(glm::max(m_llfRender, m_llfVisible),
        // glm::min(m_urbRender, m_urbVisible), m_colorOverride);
        m_marchingCubes.run(m_llfRender, m_urbRender, m_colorOverride);

        m_vertices = m_marchingCubes.takeVertices();

        m_meshEmpty = m_vertices.empty();

        m_verticesDirty = false;
        m_llfDirty = glm::uvec3(std::numeric_limits<uint32_t>::max());
        m_urbDirty = glm::uvec3(0);

        return true;
    }

    return false;
}
