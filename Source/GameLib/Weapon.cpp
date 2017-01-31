#include "Weapon.h"

Weapon::Weapon(const WeaponConfig & config, HailstormManager & hailstormManager, VoxelObjectWorldUID creatorUID):
    m_config(config),
    m_hailstormManager(hailstormManager),
    m_creatorUID(creatorUID)
{

}

void Weapon::setFireRequest(bool active, const glm::vec3 & target)
{
    m_fireRequestActive = active;
    m_fireRequestTarget = target;
}

void Weapon::setPosition(const glm::vec3 & position)
{
    m_position = position;
}

void Weapon::update(float seconds, const glm::vec3 & position)
{
    if (!m_fireRequestActive)
    {
        m_position = position;
        m_cooldown = std::max(0.0f, m_cooldown - seconds);
        return;
    }

    auto timeAccumulator = 0.0f;
    auto velocity = (position - m_position) / seconds;

    auto baseMillis = (CurrentMillis() - ((TimestampMillis)std::ceil(seconds * 1000.0f)));

    while (seconds > m_cooldown)
    {
        seconds -= m_cooldown;
        timeAccumulator += m_cooldown;

        auto intermediatePosition = m_position + velocity * timeAccumulator;

        auto bullet = HailstormParticle(intermediatePosition,
                                        glm::normalize(m_fireRequestTarget - intermediatePosition) * 400.0f,
                                        100,
                                        baseMillis + ((TimestampMillis)(timeAccumulator * 1000.0f)),
                                        2000,
                                        m_config.meshID,
                                        m_creatorUID);

        m_hailstormManager.addBullet(bullet);

        m_cooldown = m_config.cooldown;
    }

    m_position = position;
    m_cooldown -= seconds;
}