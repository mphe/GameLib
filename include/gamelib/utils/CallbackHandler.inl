#include "CallbackHandler.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::regCallback(CallbackFunction callback, void* me)
    {
        _callbacks.emplace_back(CallbackInfo(me, callback));
    };

    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::unregCallback(CallbackFunction callback, void* me)
    {
        auto it = std::find(_callbacks.begin(), _callbacks.end(), CallbackInfo(me, callback));
        if (it != _callbacks.end())
            it->remove = true;
    };

    template <class Ret, class... Args>
    template <class... Args2>
    void CallbackHandler<Ret, Args...>::call(Args2&&... args)
    {
        // Don't use iterators here, you need to maintain size and position yourself
        // otherwise problems could occur when new elements are added.
        for (size_t i = 0, size = _callbacks.size(); i < size; ++i) // save size because it could change when new elements are added
        {
            CallbackInfo* info = &_callbacks[i];
            while (info->remove)
            {
                _callbacks.erase(_callbacks.begin() + i);
                if (i >= --size) // end of list reached (new elements might have been added, but they must not be called)
                    return;
            }
            info->callback(info->me, std::forward<Args2>(args)...);
        }
    };

    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::clear()
    {
        _callbacks.clear();
    };

    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::clean()
    {
        for (auto i = _callbacks.begin(); i != _callbacks.end(); ++i)
        {
            while (i->remove)
                if ((i = _callbacks.erase(i)) == _callbacks.end())
                    return;
        }
    }

    template <class Ret, class... Args>
    size_t CallbackHandler<Ret, Args...>::size() const
    {
        return _callbacks.size();
    }
}

