#ifndef CALLBACK_HANDLER_HPP
#define CALLBACK_HANDLER_HPP

#include <vector>
#include <algorithm>

namespace gamelib
{
    template <class _Ret, class ..._Args>
    class CallbackHandler
    {
        public:
            typedef _Ret (*CallbackFunction)(void*, _Args...);

        private:
            struct CallbackInfo
            {
                void* me;
                CallbackFunction callback;

                CallbackInfo(void* me_, CallbackFunction callback_) :
                    me(me_),
                    callback(callback_)
                {}

                bool operator==(const CallbackInfo& ci) const
                {
                    return me == ci.me && callback == ci.callback;
                }
            };

        public:
            void regCallback(CallbackFunction callback, void* me = 0)
            {
                _callbacks.push_back(CallbackInfo(me, callback));
            };

            void unregCallback(CallbackFunction callback, void* me = 0)
            {
                auto it = std::find(_callbacks.begin(), _callbacks.end(), CallbackInfo(me, callback));
                _callbacks.erase(it);
            };

            void call(const _Args... args) const
            {
                for (auto& i : _callbacks)
                {
                    i.callback(i.me, args...);
                }
            };

            void clear()
            {
                _callbacks.clear();
            };

            size_t size() const
            {
                return _callbacks.size();
            }

        private:
            std::vector<CallbackInfo> _callbacks;
    };
}

#endif
