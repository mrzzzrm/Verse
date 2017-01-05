#pragma once

inline HailstormBullet::HailstormBullet(const glm::vec3 & origin,
                                        const glm::vec3 & velocity,
                                        float energy,
                                        DurationMillis lifetime):
    origin(origin),
    velocity(velocity),
    energy(energy),
    lifetime(lifetime)
{
}