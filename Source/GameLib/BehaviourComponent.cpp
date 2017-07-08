#include "BehaviourComponent.h"

#include <algorithm>

#include "AbstractBehaviour.h"

void BehaviourComponent::addBehaviour(const std::shared_ptr<AbstractBehaviour> & behaviour)
{
    m_behaviours.emplace_back(behaviour);
}

std::shared_ptr<AbstractBehaviour> BehaviourComponent::getBehaviourByName(const std::string & name)
{
    auto iter = std::find_if(m_behaviours.begin(), m_behaviours.end(), [&] (const auto & behaviour) {
        return behaviour->name() == name;
    });
    if (iter == m_behaviours.end()) return {};
    return *iter;
}