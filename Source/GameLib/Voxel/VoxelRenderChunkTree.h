#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/RandomColorGenerator.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelRenderChunk.h"

namespace deliberation
{
class Pose3D;
}

class VoxelWorld;

class VoxelRenderChunkTree final {
public:
    VoxelRenderChunkTree(const VoxelWorld & voxelWorld, const glm::uvec3 & size);

    const glm::uvec3 & size() const { return m_size; }

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
    const VoxelWorld &  m_voxelWorld;
    glm::uvec3          m_size;
    std::vector<bool>   m_nodeMask;
    std::vector<Node>   m_nodes;
    std::vector<ChunkWrapper>
                        m_chunks;
    RandomColorGenerator
                        m_colorGenerator;
};