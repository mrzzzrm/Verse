#include <iostream>

#include <Deliberation/Platform/Application.h>

#include "SandboxApplication.h"
#include "EquipmentPrototype.h"

using namespace deliberation;

class ResourceSandbox:
    public SandboxApplication
{
public:
    ResourceSandbox():
        SandboxApplication("NpcSandbox")
    {

    }

    void onSandboxStartup() override
    {
        std::ifstream i("Data/Prototypes/Drone.json");
        nlohmann::json j;
        i >> j;
        
        EquipmentPrototype equipmentPrototype(j["Equipment"]);
    }

    void onSandboxUpdate(float seconds) override
    {
    }

    void onSandboxRender() override
    {
    }
};

int main(int argc, char *argv[])
{
    return ResourceSandbox().run(argc, argv);
}
