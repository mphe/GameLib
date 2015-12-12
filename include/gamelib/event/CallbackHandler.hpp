#ifndef CALLBACK_HANDLER_HPP
#define CALLBACK_HANDLER_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include <functional>

// TODO: Consider switching to std::list for easier item removing

namespace gamelib
{
    template <class Signature>
    class CallbackHandler
    {
        public:
            typedef std::function<Signature> CallbackFunction;

        private:
            struct CallbackInfo
            {
                CallbackFunction callback;
                bool remove; // is this entry marked for removal?

                CallbackInfo(const CallbackFunction& callback_) :
                    callback(callback_),
                    remove(false)
                {}

                bool operator==(const CallbackInfo& ci) const
                {
                    return callback == ci.callback && remove == ci.remove;
                }
            };

        public:
            void regCallback(CallbackFunction callback);

            // The entry won't be erased immediatelly, because it could damage the iterators in call().
            // Instead it will be erased when calling clean() or when iterating over it inside call().
            void unregCallback(CallbackFunction callback);

            template <class... Args>
            void call(Args&&... args) const;

            void clear();

            // Iterates through the list and removes every entry marked for removal
            // DON'T CALL THIS INSIDE CALL() OR A KITTEN WILL DIE AND SEGFAULTS MAY RAIN UPON YOU!
            void clean();

            // Might not always report the correct size, because there could still be entrys marked for removal, that haven't been removed yet.
            size_t size() const;

        private:
            mutable std::vector<CallbackInfo> _callbacks;
    };

    template <class Signature>
    void CallbackHandler<Signature>::regCallback(CallbackFunction callback)
    {
        _callbacks.emplace_back(CallbackInfo(callback));
    };

    template <class Signature>
    void CallbackHandler<Signature>::unregCallback(CallbackFunction callback)
    {
        auto it = std::find(_callbacks.begin(), _callbacks.end(), CallbackInfo(callback));
        if (it != _callbacks.end())
            it->remove = true;
    };

    template <class Signature>
    template <class... Args>
    void CallbackHandler<Signature>::call(Args&&... args) const
    {
        for (auto i = _callbacks.begin(); i != _callbacks.end(); ++i)
        {
            while (i->remove)
                if ((i = _callbacks.erase(i)) == _callbacks.end())
                    return;
            i->callback(std::forward<Args>(args)...);
        }
    };

    template <class Signature>
    void CallbackHandler<Signature>::clear()
    {
        _callbacks.clear();
    };

    // Iterates through the list and removes every entry marked for removal
    // DON'T CALL THIS INSIDE CALL() OR A KITTEN WILL DIE AND SEGFAULTS MAY RAIN UPON YOU!
    template <class Signature>
    void CallbackHandler<Signature>::clean()
    {
        for (auto i = _callbacks.begin(); i != _callbacks.end(); ++i)
        {
            while (i->remove)
                if ((i = _callbacks.erase(i)) == _callbacks.end())
                    return;
        }
    }

    // Might not always report the correct size, because there could still be entrys marked for removal, that haven't been removed yet.
    template <class Signature>
    size_t CallbackHandler<Signature>::size() const
    {
        return _callbacks.size();
    }
}

#endif
