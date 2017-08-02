#pragma once

#include <unordered_map>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Resource/ResourceToken.h>

#include "EmitterInstance.h"
#include "R.h"
#include "VfxParticle.h"
#include "VfxMeshRenderer.h"
#include "VfxPointLightManager.h"

class ResourceManager;

class VfxManager final
{
public:
    VfxManager(
        RenderManager & renderManager, ResourceManager & resourceManager);

    VfxMeshRenderer &       renderer();
    const VfxMeshRenderer & renderer() const;

    VfxMeshId getOrCreateMeshId(const ResourceToken & resourceToken);

    VfxParticleId addParticle(const VfxParticle & particle);

    /**
     * Will not physically remove the particle, this only happens when it expires, and will hide it until then
     * @param particleId
     */
    void          disengageParticle(const VfxParticleId & particleId);

    void addEmitterInstance(std::shared_ptr<EmitterInstance> emitterInstance);
    void
    removeEmitterInstance(std::shared_ptr<EmitterInstance> emitterInstance);

    void update(float seconds);

private:
    struct DeathEntry
    {
        TimestampMillis timeOfDeath;
        VfxParticleId   id;

        DeathEntry(TimestampMillis timeOfDeath, const VfxParticleId & id);
        bool operator<(const DeathEntry & rhs) const;
    };

private:
    // Make sure particle meshes have RGBA colors.
    static std::shared_ptr<MeshData> processMesh(const std::shared_ptr<MeshData> & inputMesh);
    static void centerMesh(std::shared_ptr<MeshData> inputMesh);

private:
    ResourceManager &                               m_resourceManager;
    std::shared_ptr<VfxMeshRenderer>                m_meshRenderer;
    std::shared_ptr<VfxPointLightManager>           m_pointLightRenderer;
    std::unordered_map<size_t, VfxMeshId>           m_meshIdByResourceId;
    SparseVector<std::shared_ptr<EmitterInstance>>  m_emitterInstances;
    std::vector<size_t>                             m_deadEmitterInstances;
    std::priority_queue<DeathEntry>                 m_deathQueue;
};
