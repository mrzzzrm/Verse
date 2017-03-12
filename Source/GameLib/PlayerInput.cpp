#include "PlayerInput.h"

#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Scene/Camera3D.h>

#include "Player/PlayerFlightControl.h"

PlayerInput::PlayerInput(InputBase & input, Camera3D & camera, PlayerFlightControl & flightControl):
    m_input(input),
    m_camera(camera),
    m_flightControl(flightControl)
{

}

void PlayerInput::update(float seconds)
{
    glm::vec3 linearThrust;

    if (m_input.keyDown(InputBase::Key_W))
    {
        linearThrust += glm::vec3(0.0f, 0.0f, -1.0f);
    }

    if (m_input.keyDown(InputBase::Key_S))
    {
        linearThrust += glm::vec3(0.0f, 0.0f, 1.0f);
    }

    if (m_input.keyDown(InputBase::Key_D))
    {
        linearThrust += glm::vec3(1.0f, 0.0f, 0.0f);
    }

    if (m_input.keyDown(InputBase::Key_A))
    {
        linearThrust += glm::vec3(-1.0f, 0.0f, 0.0f);
    }

    m_flightControl.setLinearThrust(linearThrust);

    if (m_input.mouseButtonDown(InputBase::MouseButton_Left))
    {
        auto mouse = m_input.mousePosition();

        glm::vec3 rotationAxisLocal(
            mouse.y,
            -mouse.x,
            0.0f
        );

        m_flightControl.setAngularThrust(rotationAxisLocal);
    }
    else
    {
        m_flightControl.setAngularThrust(glm::vec3(0.0f));
    }
}