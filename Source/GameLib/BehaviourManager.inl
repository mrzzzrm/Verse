
template<typename T>
void BehaviourManager::addBehaviourType(const std::string & name)
{
    m_behaviourFactoryByName.emplace(name, [=](World & world) {
        auto behaviour = std::make_shared<T>(world);
        behaviour->setName(name);
        return behaviour;
    });
}