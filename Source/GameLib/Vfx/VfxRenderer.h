#pragma once

#include <memory>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/Mesh2.h>

#include "VfxDefines.h"
#include "VfxParticle.h"
#include "VfxRenderBatch.h"

#include "GameLib.h"

namespace deliberation
{

class Camera3D;
class Context;

}

class VfxRenderer final
{
public:
    VfxRenderer(Context & context, const Camera3D & camera);

    Context & context() const;
    const Camera3D & camera() const;

    const Program & program();
    const Buffer & globalsBuffer() const;

    VfxMeshId addMesh(const Mesh2 & mesh);

    VfxParticleId addParticle(const VfxParticle & particle);
    void removeParticle(const VfxParticleId & particle);

    void update(float seconds);
    void render();

private:
    size_t batchIndex(VfxMeshId meshId, VfxParticleOrientationType orientationType) const;

private:
    Context &           m_context;
    const Camera3D &    m_camera;

    LinearMap<std::unique_ptr<VfxRenderBatch>>
                        m_batches;
    size_t              m_meshIdCounter = 0;

    LayoutedBlob        m_globals;
    TypedBlobValueAccessor<glm::mat4>
                        m_viewProjectionGlobal;
    TypedBlobValueAccessor<uint32_t>
                        m_timeGlobal;

    Program             m_program;
    Buffer              m_globalsBuffer;
};