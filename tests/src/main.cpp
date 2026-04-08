////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: main.cpp
//
////////////////////////

#include <iostream>

#include "ecs.hpp"

int main()
{
    ecs::World world = ecs::World();

    ecs::Entity entity = world.createEntity();
    world.addComponent<int>(entity);

    for (ecs::u64 i = 0; i < 1'000'000; ++i) {
        world.edit<int>(entity, [](int &component) {
            ++component;
        });
    }

    world.edit<int>(entity, [](int &component) {
        std::cout << "Component: " << component << std::endl;
    });
    return 0;
}
