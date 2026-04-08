////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: world.inl
//
////////////////////////

#include <functional>

#include "thread/jobs.hpp"
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

    template<typename ... Ts, typename F>
        requires std::is_invocable_r_v<void, F, Ts &...>
    void World::view(F &&lambda)
    {
        auto managers = std::tuple<ComponentManager<Ts>&...>{ getComponentManager<Ts>()... };
        auto& pivot = std::get<0>(managers);
        const u64 pivot_size = pivot._components.size();

        if (!pivot_size)
            return;

        u64 num_threads = _pool.getNumThreads();
        u64 chunk_size = pivot_size / num_threads;

        if (chunk_size == 0)
            chunk_size = 1;

        Jobs jobs;

        _entity_manager.lock();
        std::apply([](auto &... manager) {
            (manager.lock(), ...);
        }, managers);

        for (u64 thread_index = 0; thread_index < num_threads; ++thread_index)
        {
            const u64 start = thread_index * chunk_size;
            const u64 end = (thread_index == num_threads - 1) ? pivot_size : (start + chunk_size);

            jobs.run(_pool, [this, start, end, &managers, &pivot, lambda] {
                for (u64 i = start; i < end; ++i)
                {
                    Entity entity = pivot._index_to_entity[i];
                    bool valid = true;

                    auto components = std::apply([&valid, &managers, &entity](auto&... mng) {
                        return std::tuple<Ts*...>{
                            ([&]() -> Ts* {
                                if (entity.id() >= mng._sparse.size() ||
                                    mng._sparse[entity.id()] == INVALID_ID) {
                                    valid = false;
                                    return nullptr;
                                }
                                return &mng._components[mng._sparse[entity.id()]];
                            }())...
                        };
                    }, managers);

                    if (!valid)
                        continue;

                    std::apply([&](Ts*... comps) {
                        lambda(*comps...);
                    }, components);
                }
            });
        }

        jobs.wait();

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
