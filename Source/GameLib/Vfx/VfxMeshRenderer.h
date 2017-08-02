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
#include "VfxMeshRenderBatch.h"

#include "GameLib.h"

namespace deliberation
{
class Camera3D;
class DrawContext;

} // namespace deliberation

class VfxRenderNode;

class VfxMeshRenderer : public Renderer
{
public:
    explicit VfxMeshRenderer(RenderManager & renderManager);

    const Program & program();
    const Buffer &  globalsBuffer() const;

    size_t getOrCreateBatchIndex(const VfxBatchKey & key);

    VfxMeshId addMesh(const std::shared_ptr<MeshData> & mesh);

    size_t addParticle(const VfxParticle & particle);

    /**
     * Physically removes the particle, should only be called from VfxManager
     * @param particleId
     */
    void   removeParticle(const VfxParticleId & particleId);

    /**
     * Hides the particle until it expires
     * @param particleId
     */
    void   disengageParticle(const VfxParticleId & particleId);

    void onRegisterRenderNodes() override;

    void onBeforeRender() override;

private:
    friend class VfxRenderNode;

private:
    std::vector<std::shared_ptr<VfxMeshRenderBatch>> m_batches;
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