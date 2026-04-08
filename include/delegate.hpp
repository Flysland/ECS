////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: delegate.hpp
//
////////////////////////

#pragma once

#include "types.hpp"

#define DECLARE_BIND_DELEGATE(delegate, var_name) \
    template<typename T> \
    void bindTo##delegate(T *object, delegate::Method<T> method) \
    { \
        var_name.bind(object, method); \
    }

#define DECLARE_UNBIND_DELEGATE(delegate, var_name) \
    template<typename T> \
    void unbindTo##delegate(T *object, delegate::Method<T> method) \
    { \
        var_name.unbind(object, method); \
    }

namespace ecs
{
    template<typename ... Args>
    struct DelegateCallData
    {
        void *object;
        void *method;
        StaticMethod<void, void *, void *, Args ...> wrapper;
    };

    template<typename ... Args>
    class Delegate
    {
        public:
            template<typename T>
            using Method = Method<T, void, Args...>;

            void clear();

            template<typename T>
            void bind(T *object, Method<T> method);

            template<typename T>
            void unbind(T *object, Method<T> method);

            void broadcast(Args &&... args);

        private:
            std::vector<DelegateCallData<Args...>> _calls;

            template<typename T>
            static void callWrapper(void *object, void *method, Args &&... args);
    };
}

#include "inl/delegate.inl"
