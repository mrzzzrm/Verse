#include "AbstractEntityBehaviour.h"

void AbstractEntityBehaviour::setEntity(const Entity & entity) {
    if (entity == m_entity) return;
    m_entity = entity;
    onEntityChanged();
}