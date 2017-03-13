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
    glm::vec3 angularThrust;

    if (m_input.keyDown(InputBase::Key_W)) linearThrust += glm::vec3(0.0f, 0.0f, -1.0f);
    if (m_input.keyDown(InputBase::Key_S)) linearThrust += glm::vec3(0.0f, 0.0f, 1.0f);
    if (m_input.keyDown(InputBase::Key_D)) linearThrust += glm::vec3(1.0f, 0.0f, 0.0f);
    if (m_input.keyDown(InputBase::Key_A)) linearThrust += glm::vec3(-1.0f, 0.0f, 0.0f);

    if (m_input.keyDown(InputBase::Key_Q)) angularThrust.z = 1;
    if (m_input.keyDown(InputBase::Key_E)) angularThrust.z = -1;

    m_flightControl.setLinearThrust(linearThrust);

    if (m_input.mouseButtonDown(InputBase::MouseButton_Left))
    {
        auto mouse = m_input.mousePosition();

        angularThrust.x = mouse.y;
        angularThrust.y = -mouse.x;

        m_flightControl.setAngularThrust(angularThrust);
    }
    else
    {
        m_flightControl.setAngularThrust(angularThrust);
    }


}