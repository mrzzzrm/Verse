#include "FlightControlBase.h"

#include <cmath>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

FlightControlBase::FlightControlBase(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config):
    m_body(body),
    m_config(config)
{}

const std::shared_ptr<RigidBody> & FlightControlBase::body() const
{
    return m_body;
}

const FlightControlConfig & FlightControlBase::config() const
{
    return m_config;
}
