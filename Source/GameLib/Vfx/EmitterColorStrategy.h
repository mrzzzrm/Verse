#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"

class EmitterColorStrategy
{
public:
    struct Result
    {
        glm::vec4 birthRGBA;
        glm::vec4 deathRGBA;
    };

public:
    virtual ~EmitterColorStrategy() = default;
    virtual Result generate() const = 0;
};

class EmitterConstColor : public EmitterColorStrategy
{
public:
    EmitterConstColor(const glm::vec4 & color);
    EmitterColorStrategy::Result generate() const override;

private:
    glm::vec4 m_color;
};

class EmitterColorOverLifetime : public EmitterColorStrategy
{
public:
    EmitterColorOverLifetime(
        const glm::vec4 & birthRGBA, const glm::vec4 & deathRGBA);
    EmitterColorStrategy::Result generate() const override;

private:
    glm::vec4 m_deathRGBA;
    glm::vec4 m_birthRGBA;
};