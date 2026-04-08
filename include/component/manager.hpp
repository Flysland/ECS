////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: manager.hpp
//
////////////////////////

#pragma once

#include "types.hpp"
#include "component/component.hpp"
#include "component/imanager.hpp"
#include "entity/entity.hpp"

namespace ecs
{
    template<typename T>
    class ComponentManager final : public IComponentManager
    {
        public:
            bool contains(const Entity &entity) const;
            void add(const Entity &entity);
            void remove(const Entity &entity);
            void lock();
            void unlock();
            u64 size() const;
            Component<T> &get(const Entity &entity);

        private:
            Components<T> _components;
            std::vector<ID> _sparse;
            Entities _index_to_entity;
            Entities _pending_additions;
            Entities _pending_removals;
            u64 _lock_level;

            bool isLocked() const;
            void addDefinitively(const Entity &entity);
            void removeDefinitively(const Entity &entity);
    };
}

#include "component/inl/manager.inl"
