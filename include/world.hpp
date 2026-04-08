////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: world.hpp
//
////////////////////////

#pragma once

#include "entity/entity.hpp"
#include "entity/manager.hpp"

namespace ecs
{
    class World
    {
        public:
            void clear();

            Entity createEntity();
            void destroyEntity(const Entity &entity);
            bool isEntityAlive(const Entity &entity) const;

        private:
            EntityManager _entity_manager;
    };
}
