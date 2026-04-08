////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: main.cpp
//
////////////////////////

#include <iostream>
#include <x86intrin.h>

#include "ecs.hpp"

struct Transform {
    float x, y, z;
    float rotX, rotY, rotZ;
    float scaleX, scaleY, scaleZ;
};

struct Physics {
    float velocity[3];
    float acceleration[3];
    float force[3];
};

int main()
{
    std::cout << "Hello from entry point!" << std::endl;

    ecs::World world = ecs::World();
    ecs::u64 num_entities = 100'000;

    std::cout << "Creating entities" << std::endl;
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
    uint64_t cycles_start = __rdtsc();

    ecs::Entities entities = ecs::Entities();
    for (ecs::u64 i = 0; i < num_entities; ++i)
        entities.push_back(world.createEntity());

    uint64_t cycles_end = __rdtsc();
    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::cout << " - Done: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << "ms" << std::endl;
    std::cout << " - cycles: " << (cycles_end - cycles_start) << " -> " << ((cycles_end - cycles_start) / num_entities) << std::endl;

    std::cout << "Adding components" << std::endl;
    time_begin = std::chrono::steady_clock::now();
    cycles_start = __rdtsc();
    for (ecs::Entity &entity : entities) {
        world.addComponent<Transform>(entity);
        world.addComponent<Physics>(entity);
    }
    cycles_end = __rdtsc();
    time_end = std::chrono::steady_clock::now();
    std::cout << " - Done: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << "ms" << std::endl;
    std::cout << " - cycles: " << (cycles_end - cycles_start) << " -> " << ((cycles_end - cycles_start) / num_entities) << std::endl;

    std::cout << "Editing components" << std::endl;
    std::atomic<int> count = 0;
    time_begin = std::chrono::steady_clock::now();
    cycles_start = __rdtsc();

    for (ecs::u64 i = 0; i < 100; ++i) {
        volatile float sink = 0;
        world.view<Transform, Physics>([&](Transform &t, Physics &p) {
            sink += t.x + p.velocity[0];
        });
    }
    cycles_end = __rdtsc();
    time_end = std::chrono::steady_clock::now();
    std::cout << "Count: " << count.load() << std::endl;
    std::cout << " - Done: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << "ms" << std::endl;
    std::cout << " - cycles: " << (cycles_end - cycles_start) << " -> " << ((cycles_end - cycles_start) / num_entities / 100) << std::endl;
    std::cout << "\rAll jobs finished!" << std::endl;
    return 0;
}
