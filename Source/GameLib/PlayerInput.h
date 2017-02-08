#pragma once

#include <memory>

#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Platform/Input.h>

#include "GameLib.h"

namespace deliberation
{

class Camera3D;

}

class PlayerFlightControl;

class PlayerInput final
{
public:
    PlayerInput(InputBase & input, Camera3D & camera, PlayerFlightControl & flightControl);

    void update(float seconds);

private:
    InputBase &     m_input;
    Camera3D &      m_camera;
    PlayerFlightControl & m_flightControl;
};