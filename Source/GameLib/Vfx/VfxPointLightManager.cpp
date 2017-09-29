#include "VfxPointLightManager.h"

#include <Deliberation/Core/UpdateFrame.h>

VfxPointLightManager::VfxPointLightManager(const std::shared_ptr<PointLightRenderer> & pointLightRenderer):
    m_pointLightRenderer(pointLightRenderer)
{

}

size_t VfxPointLightManager::addParticlePointLight(const VfxParticle & particle,
                                                   const VfxParticleId & particleId,
                                                   const VfxPointLightDesc & particlePointLightDesc)
{
    const auto birthRgb = glm::vec3(particle.birthRGBA.r,
                                    particle.birthRGBA.g,
                                    particle.birthRGBA.b);
    const auto deathRgb = glm::vec3(particle.deathRGBA.r,
                                    particle.deathRGBA.g,
                                    particle.deathRGBA.b);

    PointLight pointLight;
    pointLight.position = particle.origin;
    pointLight.intensity = particlePointLightDesc.intensity * birthRgb;

    const auto pointLightIndex = m_pointLightRenderer->addPointLight(pointLight);

    const auto particlePointLightIndex = m_pointLights.emplace();
    auto & particlePointLight = m_pointLights[particlePointLightIndex];

    particlePointLight.desc = particlePointLightDesc;
    particlePointLight.velocity = particle.velocity;
    particlePointLight.pointLight = pointLightIndex;
    particlePointLight.birthRgb = birthRgb;
    particlePointLight.deathRgb = deathRgb;
    particlePointLight.particleUID = particleId.uid;

    return particlePointLightIndex;
}

void VfxPointLightManager::removeParticlePointLight(const VfxParticleId & particleId)
{
    const auto index = particleId.particlePointLight;

    auto & particlePointLight = m_pointLights[index];
    m_pointLightRenderer->removePointLight(particlePointLight.pointLight);

    m_pointLights.erase(index);
}

void VfxPointLightManager::disengageParticlePointLight(const VfxParticleId & particleId)
{
    const auto index = particleId.particlePointLight;

    // This could mean the particle expired and was removed in this frame and someone tries to disengage it manually now
    if (!m_pointLights.contains(index) || m_pointLights[index].particleUID != particleId.uid) return;

    auto & particlePointLight = m_pointLights[index];
    auto & pointLight = m_pointLightRenderer->pointLight(particlePointLight.pointLight);
    pointLight.active = false;
}

void VfxPointLightManager::update(const UpdateFrame & updateFrame)
{
    for (auto & particlePointLight : m_pointLights)
    {
        auto & pointLight = m_pointLightRenderer->pointLight(particlePointLight.pointLight);

        pointLight.position += particlePointLight.velocity * updateFrame.gameSeconds();
    }
}
