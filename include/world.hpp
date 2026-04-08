////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: world.hpp
//
////////////////////////

#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>

#include "entity/entity.hpp"
#include "entity/manager.hpp"
#include "component/manager.hpp"

namespace ecs
{
    class World
    {
        using ComponentManagers = std::unordered_map<std::type_index, std::unique_ptr<IComponentManager>>;

        public:
            void clear();

            Entity createEntity();
            void destroyEntity(const Entity &entity);
            bool isEntityAlive(const Entity &entity) const;

            template<typename T>
            void addComponent(const Entity &entity);

            template<typename T>
            void removeComponent(const Entity &entity);

            template<typename T>
            bool containsComponent(const Entity &entity);

        private:
            EntityManager _entity_manager;
            ComponentManagers _component_managers;

            template<typename T>
            void registerComponent();

            template<typename T>
            void unregisterComponent();

            template<typename T>
            bool componentIsRegistered() const;

            template<typename T>
            ComponentManager<T> &getComponentManager();
    };
}

#include "inl/world.inl"
