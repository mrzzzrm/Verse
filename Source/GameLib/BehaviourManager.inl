
template<typename T>
void BehaviourManager::addBehaviourType(const std::string & name)
{
    m_behaviourFactoryByName.emplace(name, [=]()
    {
        auto behaviour = std::make_shared<T>();
        behaviour->setName(name);
        return behaviour;
    });
}