#include "VoxelRenderChunkTree.h"

#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/ScopeProfiler.h>

VoxelRenderChunkTree::VoxelRenderChunkTree(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_size(size)
{
    Assert(m_size.x >= 0 && m_size.y >= 0 && m_size.z >= 0, "");

    size_t numChunks = 0;
    size_t numNodes = 0;

    glm::uvec3 maxChunkSize(16); // Must be > 2

    std::function<void(u32, const glm::ivec3 &, const glm::ivec3 &)> buildTree = [&] (
        u32 index,
        const glm::uvec3 & llf,
        const glm::uvec3 & size)
    {
        if (index >= m_nodes.size()) m_nodes.resize(index + 1);

        auto & node = m_nodes[index];

        node.llf = llf;
        node.urb = glm::ivec3(llf + size) - glm::ivec3(1);

        if (size.x <= maxChunkSize.x &&
            size.y <= maxChunkSize.y &&
            size.z <= maxChunkSize.z)
        {
            node.chunk = numChunks;
            numChunks++;
            node.leaf = true;
        }
        else
        {
            u8 longestAxis = 0;

            if (size.x >= size.y && size.x >= size.z) longestAxis = 0;
            else if (size.y >= size.x && size.y >= size.z) longestAxis = 1;
            else longestAxis = 2;

            i32 sharedIndex = size[longestAxis] / 2;

            auto leftSize = size;
            leftSize[longestAxis] = sharedIndex + 1 ;

            auto rightSize = size;
            rightSize[longestAxis] -= sharedIndex;

            buildTree(index * 2 + 1, llf, leftSize);

            glm::uvec3 llfOffset;
            llfOffset[longestAxis] = leftSize[longestAxis] - 1;

            buildTree(index * 2 + 2, llf + llfOffset, rightSize);
        }
    };

    buildTree(0, glm::ivec3(0), m_size);

    m_chunks.resize(numChunks);
    m_nodeMask.resize(m_nodes.size());
}


void VoxelRenderChunkTree::addVoxels(const std::vector<Voxel> & voxels)
{
    ScopeProfiler scopeProfiler("VoxelRenderChunkTree::addVoxels()");

    for (auto & voxel : voxels)
    {
        addVoxelToNode(0, voxel);
    }
}

void VoxelRenderChunkTree::removeVoxel(const std::vector<glm::uvec3> & voxels)
{
    for (auto & voxel : voxels)
    {
        removeVoxelFromNode(0, voxel);
    }
}

void VoxelRenderChunkTree::schedule(const Pose3D & pose)
{
    //ScopeProfiler scopeProfiler("VoxelRenderChunkTree::schedule()");

    for (auto & chunk : m_chunks)
    {
        if (!chunk.chunk) continue;

        Pose3D chunkPose(pose);
        chunkPose.setPosition(pose.position() + pose.orientation() * chunk.position);

        chunk.chunk->schedule(chunkPose);
    }
}

std::string VoxelRenderChunkTree::toString() const
{
    std::stringstream stream;

    std::function<void(u32, u32)> nodeToString = [&] (u32 index, u32 indent)
    {
        for (u32 i = 0; i < indent; i++) stream << "  ";

        auto & node = m_nodes[index];

        stream << "Node " << node.llf << " -> " << node.urb << std::endl;

        auto l = index * 2 + 1;
        auto r = index * 2 + 2;

        if (!node.leaf) nodeToString(l, indent + 1);
        if (!node.leaf) nodeToString(r, indent + 1);
    };

    nodeToString(0, 0);

    return stream.str();
}

void VoxelRenderChunkTree::addVoxelToNode(u32 index, const Voxel & voxel)
{
    auto & node = m_nodes[index];

    auto c = glm::ivec3(voxel.cell);

    if (c.x < node.llf.x || c.x > node.urb.x ||
        c.y < node.llf.y || c.y > node.urb.y ||
        c.z < node.llf.z || c.z > node.urb.z)
    {
        return;
    }

    if (voxel.visible)
    {
        node.hidden = false;
    }

    node.empty = false;

    if (node.leaf)
    {
        auto & chunk = m_chunks[node.chunk];

        if (chunk.chunk)
        {
            if (chunk.chunk.use_count() > 1)
            {
                chunk.chunk = chunk.chunk->clone();
            }
        }
        else
        {
            chunk.position = node.llf;
            chunk.chunk = std::make_shared<VoxelRenderChunk>(m_voxelWorld,
                                                             glm::uvec3(node.urb - node.llf + glm::ivec3(1)));
        }

        Voxel chunkLocalVoxel = voxel;
        chunkLocalVoxel.cell -= node.llf;

        chunk.chunk->addVoxel(chunkLocalVoxel);
    }
    else
    {
        addVoxelToNode(index * 2 + 1, voxel);
        addVoxelToNode(index * 2 + 2, voxel);
    }
}

void VoxelRenderChunkTree::removeVoxelFromNode(u32 index, const glm::uvec3 & voxel)
{
    auto & node = m_nodes[index];

    if (voxel.x < node.llf.x || voxel.x > node.urb.x ||
        voxel.y < node.llf.y || voxel.y > node.urb.y ||
        voxel.z < node.llf.z || voxel.z > node.urb.z)
    {
        return;
    }

    if (node.leaf)
    {
        Assert(!!m_chunks[node.chunk].chunk, "");
        m_chunks[node.chunk].chunk->removeVoxel(voxel - glm::uvec3(node.llf));
    }
    else
    {
        removeVoxelFromNode(index * 2 + 1, voxel);
        removeVoxelFromNode(index * 2 + 2, voxel);
    }
}