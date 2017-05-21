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
    VoxelRenderChunk(VoxelWorld & voxelWorld, const glm::uvec3 & size,
                     const glm::uvec3 & llfRender, const glm::uvec3 & urbRender,
                     const Optional<glm::vec3> & colorOverride = Optional<glm::vec3>());
    VoxelRenderChunk(const VoxelRenderChunk & other);

    void addVoxel(const Voxel & voxel, bool visible);
    void removeVoxel(const glm::uvec3 & voxel, bool visible);
    void invalidateVoxel(const glm::uvec3 & voxel);
    void updateVoxelVisibility(const glm::uvec3 & voxel, bool visible);

    std::shared_ptr<VoxelRenderChunk> clone();

    void schedule(const Pose3D & pose, float scale) const;

private:
    VoxelCluster<glm::vec3>
                        m_cluster;
    VoxelWorld &        m_voxelWorld;
    mutable VoxelClusterMarchingCubes
                        m_marchingCubes;
    VoxelCluster<u8>    m_configCluster;
    u32                 m_voxelCount = 0;
    mutable bool        m_drawDirty = true;
    mutable bool        m_meshEmpty = true;
    mutable glm::uvec3  m_llfDirty{std::numeric_limits<u32>::max()};
    mutable glm::uvec3  m_urbDirty;
//    glm::uvec3          m_llfVisible;
//    glm::uvec3          m_urbVisible;
    glm::uvec3          m_llfRender;
    glm::uvec3          m_urbRender;
    mutable Draw        m_draw;
    mutable Uniform     m_transformUniform;
    mutable Uniform     m_viewUniform;
    mutable Uniform     m_projectionUniform;
    mutable Uniform     m_scaleUniform;
    size_t              m_numVisibleVoxels = 0;
    Optional<glm::vec3> m_colorOverride;
};