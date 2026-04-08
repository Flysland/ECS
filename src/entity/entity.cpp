////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: entity.cpp
//
////////////////////////

#include "entity/entity.hpp"

namespace ecs
{
    Entity::Entity()
        : _id(INVALID_ID)
        , _generation()
    { }

    Entity::Entity(ID id, u64 generation)
        : _id(id)
        , _generation(generation)
    { }

    ID Entity::id() const
    {
        return _id;
    }
}
