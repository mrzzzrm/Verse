#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelCluster.h"

class VoxelWorld;

class VoxelRenderChunk final
{
public:
    VoxelRenderChunk(const glm::uvec3 & size);

    void addVoxel(const Voxel & voxel);
    void removeVoxel(const glm::uvec3 & voxel);

    std::shared_ptr<VoxelRenderChunk> clone();

    void schedule(const VoxelWorld & voxelWorld, const Pose3D & pose);

private:
    VoxelCluster<glm::vec3>
                        m_cluster;
    VoxelCluster<u8>    m_configCluster;
    u32                 m_voxelCount = 0;
    bool                m_drawDirty = true;
    glm::uvec3          m_llfDirty;
    glm::uvec3          m_urbDirty;
    Draw                m_draw;
    Uniform             m_transformUniform;
};