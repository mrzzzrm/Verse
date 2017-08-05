#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"

class EmitterRotationStrategy
{
public:
    virtual ~EmitterRotationStrategy() = default;

    /**
     * (x,y,z) - UnitAxis, w - speed
     */
    virtual glm::vec4 generateRotation() = 0;
};

class EmitterNoRotation : public EmitterRotationStrategy
{
public:
    glm::vec4 generateRotation() override { return {1.0f, 0.0f, 0.0f, 0.0f}; /*Still keep this normalized*/ }
};

class EmitterBillboardRotation : public EmitterRotationStrategy
{
public:
    EmitterBillboardRotation(float minAngularSpeed, float maxAngularSpeed):
        m_minAngularSpeed(minAngularSpeed), m_maxAngularSpeed(maxAngularSpeed)
    {}

    glm::vec4 generateRotation() override;

private:
    float m_minAngularSpeed = 0;
    float m_maxAngularSpeed = 0;
};