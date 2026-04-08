////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: types.hpp
//
////////////////////////

#pragma once

#include <vector>

#define INVALID_ID ((u64)-1)

namespace ecs
{
    using i8 = char;
    using i16 = short;
    using i32 = int;
    using i64 = long;
    using u8 = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long;

    using ID = u64;
    using IDs = std::vector<ID>;

    template<typename T, typename R, typename ... ARGS>
    using Method = R (T::*)(ARGS...);

    template<typename T, typename R, typename ... ARGS>
    using Methods = std::vector<Method<T, R, ARGS...>>;

    template<typename T, typename R, typename ... ARGS>
    using ConstMethod = R (T::*)(ARGS...) const;

    template<typename T, typename R, typename ... ARGS>
    using ConstMethods = std::vector<ConstMethod<T, R, ARGS...>>;

    template<typename R, typename ... ARGS>
    using StaticMethod = R (*)(ARGS...);

    template<typename R, typename ... ARGS>
    using StaticMethods = std::vector<StaticMethod<R, ARGS...>>;

    template<typename T, auto M, typename R, typename ... ARGS>
    concept HasMethod = requires(T &t, ARGS &&... args)
    {
        { (t.*M)(std::forward<ARGS>(args)...) } -> std::same_as<R>;
    };

    template<typename T, auto M, typename R, typename ... ARGS>
    concept HasConstMethod = requires(const T &t, ARGS &&... args)
    {
        { (t.*M)(std::forward<ARGS>(args)...) } -> std::same_as<R>;
    };

    template<typename T, auto M, typename R, typename ... ARGS>
    concept HasStaticMethod = requires(ARGS &&... args)
    {
        { T::M(std::forward<ARGS>(args)...) } -> std::same_as<R>;
    };
}
