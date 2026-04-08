////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: manager.cpp
//
////////////////////////

#include "entity/manager.hpp"

namespace ecs
{
    void EntityManager::clear()
    {
        _entities_requested_for_destruction.clear();
        _generations.clear();
        _current_id = 0;
        _available_ids.clear();
        _lock_level = 0;
        _remove_component_delegate.clear();
    }

    void EntityManager::destroyEntity(const Entity &entity)
    {
        if (isLocked()) {
            invalidateEntity(entity);
            _entities_requested_for_destruction.push_back(entity);
        } else {
            destroyEntityDefinitively(entity);
            invalidateEntity(entity);
        }
    }

    Entity EntityManager::createEntity()
    {
        ID id = generateEntityID();

        if (_generations.size() <= id)
            _generations.resize(id + 1);

        return Entity(id, _generations[id]);
    }

    bool EntityManager::isEntityAlive(const Entity &entity) const
    {
        if (_generations.size() <= entity.id())
            return false;

        return _generations[entity.id()] == entity._generation;
    }

    void EntityManager::lock()
    {
        ++_lock_level;
    }

    void EntityManager::unlock()
    {
        if (_lock_level == 0)
            return;

        if (--_lock_level)
            return;

        for (const Entity &entity : _entities_requested_for_destruction)
            destroyEntityDefinitively(entity);

        _entities_requested_for_destruction.clear();
    }

    bool EntityManager::isLocked() const
    {
        return _lock_level;
    }

    ID EntityManager::generateEntityID()
    {
        if (!_available_ids.empty()) {
            ID id = _available_ids.back();

            _available_ids.pop_back();
            return id;
        }

        if (_current_id == INVALID_ID)
            return INVALID_ID;

        return _current_id++;
    }

    void EntityManager::invalidateEntity(const Entity &entity)
    {
        if (_generations.size() <= entity.id())
            return;

        _generations[entity.id()] += 1;
    }

    void EntityManager::destroyEntityDefinitively(const Entity &entity)
    {
        _remove_component_delegate.broadcast(entity);
    }
}
