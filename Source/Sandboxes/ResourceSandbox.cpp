#include <iostream>

#include "Deliberation/Resource/ResourceManager.h"

struct Foo
{
    int i = 1;
};


int main(int argc, char * argv[]) {
    World world;
    auto resourceManager = world.addSystem<ResourceManager>();
    resourceManager->setLoader<std::vector<Foo>>([](const auto & path) {
        std::vector<Foo> f(2);
        return f;
    });

    resourceManager->resource<std::vector<Foo>>("hello");
    return 0;
}
