////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: manager.inl
//
////////////////////////

#include <cassert>
#include <iostream>

#include "component/manager.hpp"

namespace ecs
{
    template<typename T>
    bool ComponentManager<T>::contains(const Entity &entity) const
    {
        return entity.id() < _sparse.size() && _sparse[entity.id()] != INVALID_ID;
    }

    template<typename T>
    void ComponentManager<T>::add(const Entity &entity)
    {
        if (contains(entity))
            return;

        if (isLocked())
            _pending_additions.push_back(entity);
        else
            addDefinitively(entity);
    }

    template<typename T>
    void ComponentManager<T>::remove(const Entity &entity)
    {
        if (!contains(entity))
            return;

        if (isLocked())
            _pending_removals.push_back(entity);
        else
            removeDefinitively(entity);
    }

    template<typename T>
    void ComponentManager<T>::lock()
    {
        ++_lock_level;
    }

    template<typename T>
    void ComponentManager<T>::unlock()
    {
        if (_lock_level == 0)
            return;

        if (--_lock_level)
            return;

        for (const Entity &entity : _pending_additions)
            addDefinitively(entity);

        for (const Entity &entity : _pending_removals)
            removeDefinitively(entity);

        _pending_additions.clear();
        _pending_removals.clear();
    }

    template<typename T>
    u64 ComponentManager<T>::size() const
    {
        return _components.size();
    }

    template<typename T>
    Component<T> &ComponentManager<T>::get(const Entity &entity)
    {
        assert(contains(entity));
        return _components[_sparse[entity.id()]];
    }

    template<typename T>
    bool ComponentManager<T>::isLocked() const
    {
        return _lock_level;
    }

    template<typename T>
    void ComponentManager<T>::addDefinitively(const Entity &entity)
    {
        if (entity.id() >= _sparse.size())
            _sparse.resize(entity.id() + 1, INVALID_ID);

        u64 index = _components.size();
        _components.emplace_back();
        _index_to_entity.push_back(entity);
        _sparse[entity.id()] = index;
    }

    template<typename T>
    void ComponentManager<T>::removeDefinitively(const Entity &entity)
    {
        if (entity.id() >= _sparse.size())
            return;

        u64 index = _sparse[entity.id()];
        u64 last = _components.size() - 1;
        Entity last_entity = _index_to_entity[last];

        std::swap(_components[index], _components[last]);
        std::swap(_index_to_entity[index], _index_to_entity[last]);
        _sparse[last_entity.id()] = index;

        _components.pop_back();
        _index_to_entity.pop_back();
        _sparse[entity.id()] = INVALID_ID;
    }
}
