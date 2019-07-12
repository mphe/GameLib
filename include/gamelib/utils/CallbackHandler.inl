#include "CallbackHandler.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::regCallback(CallbackFunction callback, void* me)
    {
        _callbacks.emplace_back(me, callback);
    };

    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::unregCallback(CallbackFunction callback, void* me)
    {
        auto it = std::find(_callbacks.begin(), _callbacks.end(), CallbackInfo(me, callback));
        if (it != _callbacks.end())
            it->callback = nullptr;
    };

    template <class Ret, class... Args>
    template <class... Args2>
    void CallbackHandler<Ret, Args...>::call(Args2&&... args)
    {
        for (auto& info : _callbacks)
            if (info)
                info.callback(info.me, std::forward<Args2>(args)...);
        clean();
    };

    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::clear()
    {
        _callbacks.clear();
    };

    template <class Ret, class... Args>
    void CallbackHandler<Ret, Args...>::clean()
    {
        std::remove_if(_callbacks.begin(), _callbacks.end(),
                [](const CallbackInfo& info) { return info; });
    }

    template <class Ret, class... Args>
    size_t CallbackHandler<Ret, Args...>::size() const
    {
        return _callbacks.size();
    }
}

