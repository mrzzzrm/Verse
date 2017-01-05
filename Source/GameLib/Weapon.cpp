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
    m_fireRequestDirection = direction;
}

void Weapon::update(float seconds)
{
    m_cooldown = std::max(0.0f, m_cooldown - seconds);

    if (!m_fireRequestActive)
    {
        return;
    }

    auto bullet = HailstormBullet(m_fireRequestOrigin,
                                  m_fireRequestDirection * 40.0f,
                                  100,
                                  2000);

    m_hailstormManager.addBullet(bullet, m_config.meshID);

    m_cooldown = m_config.frequency;
}