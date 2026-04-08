////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: manager.hpp
//
////////////////////////

#pragma once

#include "entity/entity.hpp"
#include "delegate.hpp"

namespace ecs
{
    class EntityManager
    {
        using RemoveComponentDelegate = Delegate<const Entity &>;

        public:
            void clear();
            void destroyEntity(const Entity &entity);
            Entity createEntity();
            bool isEntityAlive(const Entity &entity) const;
            void lock();
            void unlock();

            DECLARE_BIND_DELEGATE(RemoveComponentDelegate, _remove_component_delegate)
            DECLARE_UNBIND_DELEGATE(RemoveComponentDelegate, _remove_component_delegate)

        private:
            Entities _entities_requested_for_destruction;
            std::vector<u64> _generations;
            ID _current_id;
            IDs _available_ids;
            u64 _lock_level;
            RemoveComponentDelegate _remove_component_delegate;

            bool isLocked() const;
            ID generateEntityID();
            void invalidateEntity(const Entity &entity);
            void destroyEntityDefinitively(const Entity &entity);
    };
}
