#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/RandomColorGenerator.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelRenderChunk.h"

namespace deliberation
{
class Pose3D;
}

class ColorPalette;
class VoxelWorld;

class VoxelRenderable final {
public:
    VoxelRenderable(VoxelWorld & voxelWorld,
                         const std::shared_ptr<ColorPalette> & palette,
                         const glm::uvec3 & size);

    VoxelWorld & voxelWorld() const { return m_voxelWorld; }
    const glm::uvec3 & size() const { return m_size; }
    std::shared_ptr<ColorPalette> & palette() { return m_palette; }
    const std::shared_ptr<ColorPalette> & palette() const { return m_palette; }

    void setScale(float scale);

    void addVoxel(const Voxel & voxel, bool visible);
    void removeVoxel(const glm::uvec3 & voxel, bool visible);
    void updateVoxelVisibility(const glm::uvec3 & voxel, bool visible);

    void schedule(const Pose3D & pose) const;

    std::string toString() const;

protected:
    struct Node
    {
        // Object volume the node gets voxels from
        glm::ivec3  llf;
        glm::ivec3  urb;

        // Object volume it is actually rendering
        glm::ivec3  llfRender;
        glm::ivec3  urbRender;

        size_t      numVisibleVoxels = 0;
        size_t      chunk = std::numeric_limits<size_t>::max();
        bool        leaf = false;
    };

    struct ChunkWrapper
    {
        size_t      index;
        glm::vec3   position;
        std::shared_ptr<VoxelRenderChunk>
                    chunk;
    };

private:
    void addVoxelToNode(u32 index, const Voxel & voxel, bool visible);
    void removeVoxelFromNode(u32 index, const glm::uvec3 & voxel, bool visible);
    void updateVoxelVisibilityInNode(size_t index, const glm::uvec3 & voxel, bool visible);
    bool isVoxelInNode(size_t index, const glm::uvec3 & voxel);
    bool isVoxelRenderedByNode(size_t index, const glm::uvec3 & voxel);

protected:
    VoxelWorld &                    m_voxelWorld;
    std::shared_ptr<ColorPalette>   m_palette;
    glm::uvec3                      m_size;
    std::vector<Node>               m_nodes;
    std::vector<ChunkWrapper>       m_chunks;
    RandomColorGenerator            m_colorGenerator;
    float                           m_scale = 1.0f;

    mutable bool                    m_drawInitialized = false;
    mutable Draw                    m_draw;
    mutable Buffer                  m_vertexBuffer;
    mutable Uniform                 m_transformUniform;
    mutable Uniform                 m_viewUniform;
    mutable Uniform                 m_projectionUniform;
    mutable Uniform                 m_scaleUniform;
};