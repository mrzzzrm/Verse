#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class AbstractBehaviour;

class BehaviourManager
{
public:
    BehaviourManager();
    virtual ~BehaviourManager() = default;

    template<typename T>
    void addBehaviourType(const std::string & name);

    std::shared_ptr<AbstractBehaviour> createBehaviour(const std::string & name);

private:
    std::unordered_map<std::string,
        std::function<std::shared_ptr<AbstractBehaviour>()>> m_behaviourFactoryByName;
};

#include "BehaviourManager.inl"