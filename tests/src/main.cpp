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

    std::cout << "Is alive: " << world.isEntityAlive(entity) << std::endl;

    world.destroyEntity(entity);

    std::cout << "Is alive: " << world.isEntityAlive(entity) << std::endl;

    entity = world.createEntity();

    std::cout << "Is alive: " << world.isEntityAlive(entity) << std::endl;

    world.destroyEntity(entity);

    std::cout << "Is alive: " << world.isEntityAlive(entity) << std::endl;

    return 0;
}
