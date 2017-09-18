#pragma once

#include <Deliberation/Platform/InputLayerWrapper.h>

#include "GameLib.h"

namespace deliberation
{
class UpdateFrame;
class World;
}

class AbstractBehaviour:
    public InputListener
{
public:
    AbstractBehaviour(World & world): m_world(world) {}
    virtual ~AbstractBehaviour() = default;

    const std::string & name() const { return m_name; }
    void                setName(const std::string & name) { m_name = name; }

    virtual void onCreated() {}
    virtual void onRemoved() {}


protected:
    World &     m_world;
    std::string m_name;
};