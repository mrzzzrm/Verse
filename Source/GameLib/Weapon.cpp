#include "Weapon.h"

Weapon::Weapon(const WeaponConfig & config, HailstormManager & hailstormManager):
    m_config(config),
    m_hailstormManager(hailstormManager)
{

}

void Weapon::setFireRequest(bool active, const glm::vec3 & origin, const glm::vec3 & direction)
{
    m_fireRequestActive = active;
    m_fireRequestOrigin = origin;
    m_fireRequestDirection = glm::normalize(direction);
}

void Weapon::update(float seconds)
{
    m_cooldown = std::max(0.0f, m_cooldown - seconds);

    if (m_cooldown > 0.0f || !m_fireRequestActive)
    {
        return;
    }

    auto bullet = HailstormBullet(m_fireRequestOrigin,
                                  m_fireRequestDirection * 400.0f,
                                  100,
                                  CurrentMillis(),
                                  2000,
                                  m_config.meshID);


    m_hailstormManager.addBullet(bullet);

    m_cooldown = m_config.cooldown;
}