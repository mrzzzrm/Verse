#include "VoxelRenderable.h"

#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/ECS/World.h>

#include "ColorPalette.h"
#include "VoxelWorld.h"

VoxelRenderable::VoxelRenderable(
    VoxelWorld &                          voxelWorld,
    const std::shared_ptr<ColorPalette> & palette,
    const glm::uvec3 &                    size)
    : m_voxelWorld(voxelWorld), m_palette(palette), m_size(size)
{
    Assert(m_size.x >= 0 && m_size.y >= 0 && m_size.z >= 0);

    size_t numChunks = 0;

    glm::uvec3 maxChunkSize(16); // Must be >= 2

    std::function<void(
        u32,
        const glm::ivec3 &,
        const glm::ivec3 &,
        const glm::ivec3 &,
        const glm::ivec3)>
        buildTree = [&](u32                index,
                        const glm::ivec3 & llf,
                        const glm::ivec3 & urb,
                        const glm::ivec3 & llfRender,
                        const glm::ivec3 & urbRender) {
            if (index >= m_nodes.size()) m_nodes.resize(index + 1);

            auto & node = m_nodes[index];

            node.llf = llf;
            node.urb = urb;
            node.llfRender = llfRender;
            node.urbRender = urbRender;

            auto size = urb - llf + 1;
            auto renderSize = urbRender - llfRender + 1;

            if (renderSize.x <= maxChunkSize.x &&
                renderSize.y <= maxChunkSize.y &&
                renderSize.z <= maxChunkSize.z)
            {
                node.chunk = numChunks;
                numChunks++;
                node.leaf = true;
            }
            else
            {
                auto longestAxis = 0;

                if (renderSize.x >= renderSize.y &&
                    renderSize.x >= renderSize.z)
                    longestAxis = 0;
                else if (
                    renderSize.y >= renderSize.x &&
                    renderSize.y >= renderSize.z)
                    longestAxis = 1;
                else
                    longestAxis = 2;

                auto separationIndex =
                    (llfRender[longestAxis] + urbRender[longestAxis]) / 2;

                Assert(separationIndex != llfRender[longestAxis]);

                /**
                 * Left child
                 */
                auto llfRenderLeft = llfRender;

                auto urbRenderLeft = urbRender;
                urbRenderLeft[longestAxis] = separationIndex - 1;

                auto llfLeft = glm::max(llfRenderLeft - glm::ivec3(1), llf);
                auto urbLeft = glm::min(urbRenderLeft + glm::ivec3(1), urb);

                buildTree(
                    index * 2 + 1,
                    llfLeft,
                    urbLeft,
                    llfRenderLeft,
                    urbRenderLeft);

                /**
                 * Left child
                 */
                auto llfRenderRight = llfRender;
                llfRenderRight[longestAxis] = separationIndex + 1;

                auto urbRenderRight = urbRender;

                auto llfRight = glm::max(llfRenderRight - glm::ivec3(1), llf);
                auto urbRight = glm::min(urbRenderRight + glm::ivec3(1), urb);

                buildTree(
                    index * 2 + 2,
                    llfRight,
                    urbRight,
                    llfRenderRight,
                    urbRenderRight);
            }
        };

    auto llf = glm::ivec3(0);
    auto urb = glm::ivec3(m_size) - 1;
    buildTree(0, llf, urb, llf, urb);

    m_chunks.resize(numChunks);
}

void VoxelRenderable::addVoxel(const Voxel & voxel, bool visible)
{
    addVoxelToNode(0, voxel, visible);
}

void VoxelRenderable::removeVoxel(const glm::uvec3 & voxel, bool visible)
{
    removeVoxelFromNode(0, voxel, visible);
}

void VoxelRenderable::updateVoxelVisibility(
    const glm::uvec3 & voxel, bool visible)
{
    updateVoxelVisibilityInNode(0, voxel, visible);
}

void VoxelRenderable::render(const Transform3D & transform) const
{
    /**
     * Init Draw
     */
    if (!m_drawInitialized)
    {
        m_vertexBuffer =
            GetGlobal<DrawContext>()->createBuffer(VoxelClusterMarchingCubes::vertexLayout());

        m_draw = GetGlobal<DrawContext>()->createDraw(m_voxelWorld.program());
        m_draw.addVertexBuffer(m_vertexBuffer);
        //     m_draw.state().setCullState(CullState::disabled());

        m_draw.sampler("Environment").setTexture(m_voxelWorld.envMap());
        m_draw.setFramebuffer(GetGlobal<RenderManager>()->gbuffer());
        m_draw.setBufferTexture("ColorPalette", m_palette->colorBuffer());
        m_draw.setBufferTexture("BrightnessScalePalette", m_palette->brightnessScaleBuffer());

        m_transformUniform = m_draw.uniform("Transform");
        m_viewUniform = m_draw.uniform("View");
        m_orientationUniform = m_draw.uniform("Orientation");
        m_projectionUniform = m_draw.uniform("Projection");
        m_drawInitialized = true;
    }

    /**
     * Sync palette
     */
    m_palette->sync();

    /**
     * Update Vertices
     */
    auto verticesChanged = false;
    auto numVertices = 0;

    for (auto & chunk : m_chunks)
    {
        if (!chunk.chunk) continue;

        if (chunk.chunk->updateVertices()) verticesChanged = true;

        numVertices += chunk.chunk->vertices().count();
    }

    if (verticesChanged)
    {
        m_vertexBuffer.reinit(numVertices);

        if (numVertices > 0)
        {
            m_vertexBuffer.mapped(
                BufferMapping::WriteOnly, [&](LayoutedBlob & vertices) {
                    auto       offset = 0;
                    const auto stride = vertices.layout().stride();

                    for (auto & chunk : m_chunks)
                    {
                        if (!chunk.chunk) continue;

                        const auto & chunkVertices = chunk.chunk->vertices();
                        if (chunkVertices.empty()) continue;

                        vertices.write(
                            offset * stride,
                            chunkVertices.rawData().ptr(),
                            chunkVertices.count() * stride);
                        offset += chunkVertices.count();
                    }
                });
        }
    }

    /**
     * Dispatch Draw
     */
    if (m_vertexBuffer.count() != 0)
    {
        const auto & camera = GetGlobal<RenderManager>()->mainCamera();

        m_viewUniform.set(camera.view());
        m_projectionUniform.set(camera.projection());
        m_transformUniform.set(transform.matrix());
        m_orientationUniform.set(glm::mat4_cast(transform.orientation()));

        m_draw.render();
    }
}

std::string VoxelRenderable::toString() const
{
    std::stringstream stream;

    std::function<void(u32, u32)> nodeToString = [&](u32 index, u32 indent) {
        for (u32 i = 0; i < indent; i++)
            stream << "  ";

        auto & node = m_nodes[index];

        stream << "Node(" << index << ") " << node.llf << " -> " << node.urb
               << " " << node.llfRender << " -> " << node.urbRender
               << std::endl;

        auto l = index * 2 + 1;
        auto r = index * 2 + 2;

        if (!node.leaf) nodeToString(l, indent + 1);
        if (!node.leaf) nodeToString(r, indent + 1);
    };

    nodeToString(0, 0);

    return stream.str();
}

void VoxelRenderable::addVoxelToNode(
    u32 index, const Voxel & voxel, bool visible)
{
    if (!isVoxelInNode(index, voxel.cell)) return;

    auto & node = m_nodes[index];

    // visible = visible && isVoxelRenderedByNode(index, voxel.cell);

    if (visible) node.numVisibleVoxels++;

    if (node.leaf)
    {
        auto & chunk = m_chunks[node.chunk];

        if (chunk.chunk)
        {
            if (chunk.chunk.use_count() > 1) chunk.chunk = chunk.chunk->clone();
        }
        else
        {
            chunk.index = index;
            chunk.position = node.llf;
            chunk.chunk = std::make_shared<VoxelRenderChunk>(
                *this,
                node.llf,
                glm::uvec3(node.urb - node.llf + glm::ivec3(1)),
                node.llfRender - node.llf,
                node.urbRender - node.llf /*,
                                                             Optional<glm::vec3>(m_colorGenerator.generate())*/);
        }

        Voxel chunkLocalVoxel = voxel;
        chunkLocalVoxel.cell -= node.llf;

        chunk.chunk->addVoxel(chunkLocalVoxel, visible);
    }
    else
    {
        addVoxelToNode(index * 2 + 1, voxel, visible);
        addVoxelToNode(index * 2 + 2, voxel, visible);
    }
}

void VoxelRenderable::removeVoxelFromNode(
    u32 index, const glm::uvec3 & voxel, bool visible)
{
    if (!isVoxelInNode(index, voxel)) return;

    auto & node = m_nodes[index];

    // visible = visible && isVoxelRenderedByNode(index, voxel);

    if (visible)
    {
        Assert(node.numVisibleVoxels > 0);
        node.numVisibleVoxels--;
    }

    if (node.leaf)
    {
        auto & chunk = m_chunks[node.chunk];
        Assert(!!chunk.chunk);

        if (chunk.chunk.use_count() > 1) chunk.chunk = chunk.chunk->clone();

        chunk.chunk->removeVoxel(voxel - glm::uvec3(node.llf), visible);
    }
    else
    {
        removeVoxelFromNode(index * 2 + 1, voxel, visible);
        removeVoxelFromNode(index * 2 + 2, voxel, visible);
    }
}

void VoxelRenderable::updateVoxelVisibilityInNode(
    size_t index, const glm::uvec3 & voxel, bool visible)
{
    if (!isVoxelInNode(index, voxel)) return;
    //  if (!isVoxelRenderedByNode(index, voxel)) return;

    auto & node = m_nodes[index];

    if (visible)
    {
        node.numVisibleVoxels++;
    }
    else
    {
        Assert(node.numVisibleVoxels > 0);
        node.numVisibleVoxels--;
    }

    if (node.leaf)
    {
        auto & chunk = m_chunks[node.chunk];
        Assert(!!chunk.chunk);

        if (chunk.chunk.use_count() > 1) chunk.chunk = chunk.chunk->clone();

        chunk.chunk->updateVoxelVisibility(
            voxel - glm::uvec3(node.llf), visible);
    }
    else
    {
        updateVoxelVisibilityInNode(index * 2 + 1, voxel, visible);
        updateVoxelVisibilityInNode(index * 2 + 2, voxel, visible);
    }
}

bool VoxelRenderable::isVoxelInNode(size_t index, const glm::uvec3 & voxel)
{
    auto & node = m_nodes[index];

    auto c = glm::ivec3(voxel);

    return c.x >= node.llf.x && c.x <= node.urb.x && c.y >= node.llf.y &&
           c.y <= node.urb.y && c.z >= node.llf.z && c.z <= node.urb.z;
}

bool VoxelRenderable::isVoxelRenderedByNode(
    size_t index, const glm::uvec3 & voxel)
{
    auto & node = m_nodes[index];

    auto c = glm::ivec3(voxel);

    return c.x >= node.llfRender.x && c.x <= node.urbRender.x &&
           c.y >= node.llfRender.y && c.y <= node.urbRender.y &&
           c.z >= node.llfRender.z && c.z <= node.urbRender.z;
}