#ifndef GAMELIB_SIGNAL_HPP
#define GAMELIB_SIGNAL_HPP

#include <cstddef>

// It is safe to unregister handles inside Signal.trigger(), including the
// currently executing handle or others.
// However, if the current executing handle should be unregistered, this should
// be done last. The handle should also not be overwritten or freed from memory
// during callback execution. Otherwise it is undefined behavior.
//
// Example:
//
// // safe
// void callback(void* arg, SignalHandle* handle)
// {
//     handle->unregister();    // safe
// }
//
// // safe
// void callback(void* arg, SignalHandle* handle)
// {
//     // <unregister other handlers from same signal>
//     handle->unregister();
// }
//
// // NOT SAFE
// void callback(void* arg, SignalHandle* handle)
// {
//     handle->unregister();
//     // <unregister other handlers from same signal>
// }
//
// handle = signal.connect(callback, &handle);
// signal.trigger()

namespace gamelib
{
    using SignalCallback = void(*)(void* arg, void* data);

    template <typename T, typename U>
    using NiceSignalCallback = void(*)(T* arg, U* data);


    class SignalHandle
    {
        friend class Signal;

        public:
            SignalHandle(SignalCallback callback = nullptr, void* data = nullptr);
            SignalHandle(SignalHandle&&);
            SignalHandle(const SignalHandle&) = delete;
            ~SignalHandle();

            auto unregister() -> void;
            auto setData(void* data) -> void;
            auto getData() const -> void*;

            auto operator=(SignalHandle&&)      -> SignalHandle&;
            auto operator=(const SignalHandle&) -> SignalHandle& = delete;

            auto operator()(void* arg) const -> void;

            explicit operator bool() const;

        private:
            SignalCallback _callback;
            void* _data;
            SignalHandle* _prev;
            SignalHandle* _next;

    };

    class Signal
    {
        public:
            Signal();
            ~Signal();
            Signal(const Signal&) = delete;
            Signal(Signal&& other);

            auto operator=(const Signal&) -> Signal& = delete;
            auto operator=(Signal&&)      -> Signal&;

            auto trigger(void* arg) const -> void;

            __attribute__((warn_unused_result))
            auto connect(SignalCallback callback, void* data) -> SignalHandle;

            template <typename T, typename U>
            __attribute__((warn_unused_result))
            auto connect(NiceSignalCallback<T, U> callback, U* data) -> SignalHandle
            {
                return connect((SignalCallback)callback, data);
            }

            auto size() const -> size_t;
            auto clear()      -> void;

        private:
            // holds no callback, but needed to hold other handlers
            SignalHandle _listeners;
    };

    template <typename T>
    class SignalWrapper : public Signal
    {
        public:
            auto trigger(T* arg) const -> void
            {
                Signal::trigger(arg);
            }

            template <typename U>
            __attribute__((warn_unused_result))
            auto connect(NiceSignalCallback<T, U> callback, U* data) -> SignalHandle
            {
                return Signal::connect((SignalCallback)callback, data);
            }
    };
}

#endif
