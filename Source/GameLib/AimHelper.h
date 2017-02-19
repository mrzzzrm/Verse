#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"

namespace deliberation
{
class Camera3D;
class PhysicsWorld;
}

class AimHelper final
{
public:
    AimHelper(const Camera3D & camera, const PhysicsWorld & physicsWorld);

    glm::vec3 getTarget(const glm::vec2 & mouse, bool & hit);

private:
    const Camera3D &        m_camera;
    const PhysicsWorld &    m_physicsWorld;
};