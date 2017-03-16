#include "Allegiance.h"

Allegiance::Allegiance(const std::string & faction):
    m_faction(faction)
{}

const std::string & Allegiance::faction() const
{
    return m_faction;
}