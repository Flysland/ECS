////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: world.cpp
//
////////////////////////

#include "world.hpp"

namespace ecs
{
    void World::clear()
    {
        _entity_manager.clear();
    }

    Entity World::createEntity()
    {
        return _entity_manager.createEntity();
    }

    void World::destroyEntity(const Entity &entity)
    {
        _entity_manager.destroyEntity(entity);
    }

    bool World::isEntityAlive(const Entity &entity) const
    {
        return _entity_manager.isEntityAlive(entity);
    }
}
