#pragma once

#include <Deliberation/Physics/Contacts/Contact.h>

#include "GameLib.h"

class VoxelClusterContact:
    public Contact
{
public:
    VoxelClusterContact(RigidBody & bodyA, RigidBody & bodyB);

    void update() override;

};