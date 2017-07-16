#pragma once

class EmitterSizeStrategy
{
public:
    struct Result
    {
        float birthScale = 1.0f;
        float deathScale = 1.0f;
    };

public:
    virtual ~EmitterSizeStrategy() = default;
    virtual Result generate() const = 0;
};

class EmitterSizeOverLifetime : public EmitterSizeStrategy
{
public:
    EmitterSizeOverLifetime(float scale);
    EmitterSizeOverLifetime(float birthScale, float deathScale);

    EmitterSizeStrategy::Result generate() const override;

private:
    float m_birthScale;
    float m_deathScale;
};