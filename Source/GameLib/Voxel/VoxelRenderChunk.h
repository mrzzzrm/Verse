#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelCluster.h"
#include "VoxelClusterMarchingCubes.h"

class VoxelWorld;

class VoxelRenderChunk final
{
public:
    VoxelRenderChunk(const VoxelWorld & voxelWorld, const glm::uvec3 & size);

    void addVoxel(const Voxel & voxel);
    void removeVoxel(const glm::uvec3 & voxel);

    std::shared_ptr<VoxelRenderChunk> clone();

    void schedule(const Pose3D & pose);

private:
    VoxelCluster<glm::vec3>
                        m_cluster;
    const VoxelWorld &  m_voxelWorld;
    VoxelClusterMarchingCubes
                        m_marchingCubes;
    VoxelCluster<u8>    m_configCluster;
    u32                 m_voxelCount = 0;
    bool                m_drawDirty = true;
    glm::uvec3          m_llfDirty;
    glm::uvec3          m_urbDirty;
    glm::uvec3          m_llfVisible;
    glm::uvec3          m_urbVisible;
    Draw                m_draw;
    Uniform             m_transformUniform;
    Uniform             m_viewProjectionUniform;
};