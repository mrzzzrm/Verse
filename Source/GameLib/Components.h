#pragma once

#include <memory>

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

namespace deliberation
{
class RigidBody;
}

class NpcController;
class FlightControl;
class VoxelObject;

struct VoxelObjectComponent:
    public Component<VoxelObjectComponent>
{
    VoxelObjectComponent(std::shared_ptr<VoxelObject> voxelObject): voxelObject(voxelObject) {}

    std::shared_ptr<VoxelObject> voxelObject;
};

struct RigidBodyComponent:
    public Component<RigidBodyComponent>
{
    RigidBodyComponent(std::shared_ptr<RigidBody> rigidBody): rigidBody(rigidBody) {}

    std::shared_ptr<RigidBody> rigidBody;
};

struct FlightControlComponent2:
    public Component<FlightControlComponent2>
{
    FlightControlComponent2(std::shared_ptr<FlightControl> flightControl): flightControl(flightControl) {}

    std::shared_ptr<FlightControl> flightControl;
};

struct NpcControllerComponent:
    public Component<NpcControllerComponent>
{
    NpcControllerComponent(std::shared_ptr<NpcController> npcController): npcController(npcController) {}

    std::shared_ptr<NpcController> npcController;
};
