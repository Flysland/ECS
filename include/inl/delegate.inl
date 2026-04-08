////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: delegate.inl
//
////////////////////////

#include "delegate.hpp"

namespace ecs
{
    template<typename ... Args>
    void Delegate<Args...>::clear()
    {
        _calls.clear();
    }

    template<typename ... Args>
    template<typename T>
    void Delegate<Args...>::bind(T *object, Method<T> method)
    {
        if (!object || !method)
            return;

        _calls.push_back({
            object,
            &method,
            &Delegate<Args...>::callWrapper<T>
        });
    }

    template<typename ... Args>
    template<typename T>
    void Delegate<Args...>::unbind(T *object, Method<T> method)
    {
        if (!object || !method)
            return;

        for (u64 i = 0; i < _calls.size(); ++i) {
            DelegateCallData<Args...> &call = _calls[i];
            Method<T>* stored_method = static_cast<Method<T> *>(call.method);

            if (call.object == object && *stored_method == method) {
                _calls[i] = _calls.back();
                _calls.pop_back();
            }
        }
    }

    template<typename ... Args>
    void Delegate<Args...>::broadcast(Args &&... args)
    {
        for (DelegateCallData<Args...> &call : _calls)
            call.wrapper(call.object, call.method, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    template<typename T>
    void Delegate<Args...>::callWrapper(void *object, void *method, Args &&... args)
    {
        if (!object || !method)
            return;

        Method<T> m = *static_cast<Method<T> *>(method);

        if (m)
            (static_cast<T *>(object)->*m)(std::forward<Args>(args)...);
    }
}
