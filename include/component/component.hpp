////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: component.hpp
//
////////////////////////

#pragma once

#include <vector>

namespace ecs
{
    template<typename T>
    using Component = T;

    template<typename T>
    using Components = std::vector<Component<T>>;
}
