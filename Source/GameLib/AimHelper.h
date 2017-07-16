#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"

namespace deliberation
{
class Camera3D;
class PhysicsWorld;
} // namespace deliberation

struct AimHelperResult
{
    bool                       hit = false;
    glm::vec3                  pointOfImpact;
    std::shared_ptr<RigidBody> body;
};

class AimHelper final
{
public:
    AimHelper(const Camera3D & camera, const PhysicsWorld & physicsWorld);

    AimHelperResult getTarget(const glm::vec2 & mouse);

private:
    const Camera3D &     m_camera;
    const PhysicsWorld & m_physicsWorld;
};