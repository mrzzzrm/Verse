#pragma once

#include <map>
#include <memory>
#include <type_traits>

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

class VfxRenderNode;

class VfxRenderer : public Renderer
{
public:
    explicit VfxRenderer(RenderManager & renderManager);

    const Program & program();
    const Buffer &  globalsBuffer() const;

    size_t getOrCreateBatchIndex(const VfxBatchKey & key);

    VfxMeshId addMesh(const std::shared_ptr<MeshData> & mesh);

    VfxParticleId addParticle(const VfxParticle & particle);
    void          removeParticle(const VfxParticleId & particle);

    void onRegisterRenderNodes() override;

    void onBeforeRender() override;

private:
    friend class VfxRenderNode;

private:
    std::vector<std::shared_ptr<VfxRenderBatch>> m_batches;
    std::map<VfxBatchKey, size_t>                m_batchIndexByKey;
    std::vector<std::shared_ptr<MeshData>>       m_meshData;
    // TODO() remove underlying type when switching std libs
    std::unordered_map<std::underlying_type<RenderPhase>::type, std::shared_ptr<VfxRenderNode>>
                                                 m_renderNodesByRenderPhase;

    LayoutedBlob                      m_globals;
    TypedBlobValueAccessor<glm::mat4> m_viewGlobal;
    TypedBlobValueAccessor<glm::mat4> m_projectionGlobal;
    TypedBlobValueAccessor<uint32_t>  m_timeGlobal;

    Program m_program;
    Buffer  m_globalsBuffer;
};