////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: entity.hpp
//
////////////////////////

#pragma once

#include "types.hpp"

namespace ecs
{
    class Entity
    {
        friend class EntityManager;

        public:
            Entity();

            ID id() const;

        private:
            ID _id;
            u64 _generation;

            Entity(ID id, u64 generation);
    };

    using Entities = std::vector<Entity>;
}
