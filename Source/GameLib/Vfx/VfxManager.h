#pragma once

#include <unordered_map>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Resource/ResourceToken.h>
#include <Deliberation/Resource/ResourceManager.h>

#include "EmitterInstance.h"
#include "VfxParticle.h"
#include "VfxMeshRenderer.h"
#include "VfxPointLightManager.h"

class VfxManager final
{
public:
    VfxManager();

    VfxMeshRenderer &       meshRenderer();
    const VfxMeshRenderer & meshRenderer() const;

    const std::shared_ptr<VfxPointLightManager> & pointLightManager() { return m_pointLightManager; }
    std::shared_ptr<const VfxPointLightManager> pointLightManager() const { return m_pointLightManager; }

    const SparseVector<std::shared_ptr<EmitterInstance>> & emitterInstances() const { return m_emitterInstances; }

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

    /**
     * Call after Emitter Prototypes have changed
     */
    void rebuildEmitterInstances();

    void update(const UpdateFrame & updateFrame);

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
    std::shared_ptr<VfxMeshRenderer>                m_meshRenderer;
    std::shared_ptr<VfxPointLightManager>           m_pointLightManager;
    std::unordered_map<size_t, VfxMeshId>           m_meshIdByResourceId;
    SparseVector<std::shared_ptr<EmitterInstance>>  m_emitterInstances;
    std::vector<size_t>                             m_deadEmitterInstances;
    std::priority_queue<DeathEntry>                 m_deathQueue;
};
