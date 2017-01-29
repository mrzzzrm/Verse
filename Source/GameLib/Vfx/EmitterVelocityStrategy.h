#pragma once

class EmitterVelocityStrategy
{
public:
    virtual ~EmitterVelocityStrategy();
    virtual glm::vec3 generateVelocity() const;
};

class EmitterConeStrategy
{
public:
    EmitterConeStrategy()
};