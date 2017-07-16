#pragma once

#include <memory>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/MeshData.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>

#include "VfxDefines.h"
#include "VfxParticle.h"
#include "VfxRenderBatch.h"

#include "GameLib.h"

namespace deliberation
{
class Camera3D;
class DrawContext;

} // namespace deliberation

class VfxRenderer : public Renderer
{
public:
    VfxRenderer(RenderManager & renderManager);

    const Program & program();
    const Buffer &  globalsBuffer() const;

    VfxMeshId addMesh(const MeshData & mesh);

    VfxParticleId addParticle(const VfxParticle & particle);
    void          removeParticle(const VfxParticleId & particle);

    void registerRenderNodes() override;

private:
    friend class VfxAlphaRenderNode;

private:
    size_t batchIndex(
        VfxMeshId meshId, VfxParticleOrientationType orientationType) const;

private:
    LinearMap<std::unique_ptr<VfxRenderBatch>> m_batches;
    size_t                                     m_meshIdCounter = 0;

    LayoutedBlob                      m_globals;
    TypedBlobValueAccessor<glm::mat4> m_viewProjectionGlobal;
    TypedBlobValueAccessor<uint32_t>  m_timeGlobal;

    Program m_program;
    Buffer  m_globalsBuffer;
};