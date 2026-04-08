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

    for (ecs::u64 i = 0; i < 100'000'000; ++i) {
        ecs::Entity entity = world.createEntity();
        world.addComponent<int>(entity);
    }
    return 0;
}
