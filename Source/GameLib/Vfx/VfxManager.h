#pragma once

#include <unordered_map>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Resource/ResourceToken.h>

#include "EmitterInstance.h"
#include "R.h"
#include "VfxParticle.h"
#include "VfxRenderer.h"

class ResourceManager;

class VfxManager final
{
public:
    VfxManager(
        RenderManager & renderManager, ResourceManager & resourceManager);

    VfxRenderer &       renderer();
    const VfxRenderer & renderer() const;

    VfxMeshId getOrCreateMeshId(const ResourceToken & resourceToken);

    VfxParticleId addParticle(const VfxParticle & particle);
    void          removeParticle(VfxParticleId particle);

    void addEmitterInstance(std::shared_ptr<EmitterInstance> emitterInstance);
    void
    removeEmitterInstance(std::shared_ptr<EmitterInstance> emitterInstance);

    void update(float seconds);

private:
    // Make sure particle meshes have RGBA colors.
    static std::shared_ptr<MeshData> processMesh(const std::shared_ptr<MeshData> & inputMesh);

private:
    ResourceManager &            m_resourceManager;
    std::shared_ptr<VfxRenderer> m_renderer;

    std::unordered_map<size_t, VfxMeshId> m_meshIdByResourceId;

    SparseVector<std::shared_ptr<EmitterInstance>> m_emitterInstances;

    std::vector<size_t> m_deadEmitterInstances;
};
