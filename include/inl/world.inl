////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: world.inl
//
////////////////////////

#include "world.hpp"

namespace ecs
{
    template<typename T>
    void World::addComponent(const Entity &entity)
    {
        if (!isEntityAlive(entity))
            return;

        return getComponentManager<T>().add(entity);
    }

    template<typename T>
    void World::removeComponent(const Entity &entity)
    {
        if (!isEntityAlive(entity) || !componentIsRegistered<T>())
            return;

        ComponentManager<T> &manager = getComponentManager<T>();

        manager.remove(entity);

        if (!manager.size())
            unregisterComponent<T>();
    }

    template<typename T>
    bool World::containsComponent(const Entity &entity)
    {
        if (!componentIsRegistered<T>())
            return false;

        return getComponentManager<T>().contains(entity);
    }

    template<typename T>
    void World::registerComponent()
    {
        auto it = _component_managers.find(std::type_index(typeid(T)));

        if (it != _component_managers.end())
            return;

        _component_managers.emplace(std::type_index(typeid(T)), std::make_unique<ComponentManager<T>>());
        _entity_manager.bindToRemoveComponentDelegate(this, &World::removeComponent<T>);
    }

    template<typename T>
    void World::unregisterComponent()
    {
        auto it = _component_managers.find(std::type_index(typeid(T)));

        if (it == _component_managers.end())
            return;

        _component_managers.erase(it);
        _entity_manager.unbindToRemoveComponentDelegate(this, &World::removeComponent<T>);
    }

    template<typename T>
    bool World::componentIsRegistered() const
    {
        return _component_managers.find(std::type_index(typeid(T))) != _component_managers.end();
    }

    template<typename T>
    ComponentManager<T> &World::getComponentManager()
    {
        auto it = _component_managers.find(std::type_index(typeid(T)));

        if (it == _component_managers.end()) {
            registerComponent<T>();
            return getComponentManager<T>();
        }

        return *static_cast<ComponentManager<T> *>(it->second.get());
    }
}
