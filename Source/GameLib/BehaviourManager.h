#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class AbstractBehaviour;

class BehaviourManager
{
public:
    BehaviourManager();
    virtual ~BehaviourManager() = default;

    template<typename T>
    void addBehaviourType(const std::string & name);

    std::shared_ptr<AbstractBehaviour>
    createBehaviour(const std::string & name, World & world);

private:
    std::unordered_map<
        std::string,
        std::function<std::shared_ptr<AbstractBehaviour>(World &)>>
        m_behaviourFactoryByName;
};

#include "BehaviourManager.inl"