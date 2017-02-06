#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "FlightControlConfig.h"

class FlightControlBase
{
public:
    FlightControlBase(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config);
    virtual ~FlightControlBase() = default;

    const std::shared_ptr<RigidBody> & body() const;
    const FlightControlConfig & config() const;

    virtual void update(float seconds) = 0;

protected:
    std::shared_ptr<RigidBody>  m_body;
    FlightControlConfig         m_config;
};