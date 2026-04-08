////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: world.inl
//
////////////////////////

#include <functional>
#include <iostream>

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

    template<typename ... Ts, typename F>
        requires std::is_invocable_r_v<void, F, Ts &...>
    void World::edit(const Entity &entity, F &&lambda)
    {
        if (!isEntityAlive(entity))
            return;

        auto managers = std::tuple<ComponentManager<Ts> &...>{ getComponentManager<Ts>()... };
        auto components = std::apply([&entity](auto &... manager) {
            return std::forward_as_tuple(manager.get(entity)...);
        }, managers);

        _entity_manager.lock();
        std::apply([](auto &... manager) {
            (manager.lock(), ...);
        }, managers);

        std::apply([&lambda](auto &... component) {
            std::invoke(lambda, component...);
        }, components);

        _entity_manager.unlock();
        std::apply([](auto &... manager) {
            (manager.unlock(), ...);
        }, managers);
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
