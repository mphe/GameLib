#ifndef GAMELIB_LIFETIMETRACKER_HPP
#define GAMELIB_LIFETIMETRACKER_HPP

#include "gamelib/utils/SlotMap.hpp"

namespace gamelib
{
    typedef SlotKeyShort LifetimeHandle;

    class LifetimeTrackerManager
    {
        public:
            LifetimeTrackerManager();

            static auto add(void* ptr)             -> LifetimeHandle;
            static auto del(LifetimeHandle handle) -> void;
            static auto get(LifetimeHandle handle) -> void*;
            static auto update(LifetimeHandle handle, void* newptr) -> void;

        private:
            static SlotMapShort<void*> _data;
    };

    template <typename T>
    class LifetimeReference;

    template <typename T>
    class LifetimeTracker
    {
        public:
            typedef LifetimeReference<T> LTRef;

        public:
            LifetimeTracker()
            {
                _register();
            }

            LifetimeTracker(const LifetimeTracker<T>& rhs)
            {
                *this = rhs;
            }

            LifetimeTracker(LifetimeTracker<T>&& rhs)
            {
                *this = std::move(rhs);
            }

            virtual ~LifetimeTracker()
            {
                _unregister();
            }

            auto operator=(const LifetimeTracker<T>&) -> LifetimeTracker<T>&
            {
                _unregister();
                _register();
                return *this;
            }

            auto operator=(LifetimeTracker<T>&& rhs) -> LifetimeTracker<T>&
            {
                _unregister();
                _lifetimehandle = rhs._lifetimehandle;
                rhs._lifetimehandle.reset();
                LifetimeTrackerManager::update(_lifetimehandle, static_cast<T*>(this));
                return *this;
            }


            auto getLTHandle() const -> LifetimeHandle
            {
                return _lifetimehandle;
            }

            auto getLTReference() const -> LTRef
            {
                return _lifetimehandle;
            }

        private:
            auto _register() -> void
            {
                _lifetimehandle = LifetimeTrackerManager::add(static_cast<T*>(this));
            }

            auto _unregister() -> void
            {
                LifetimeTrackerManager::del(_lifetimehandle);
                _lifetimehandle.reset();
            }

        private:
            LifetimeHandle _lifetimehandle;
    };


    template <typename T>
    class LifetimeReference
    {
        public:
            LifetimeReference() {}
            LifetimeReference(LifetimeHandle handle_) : handle(handle_) {}
            LifetimeReference(T const* ptr)
            {
                this->assign(ptr);
            }

            LifetimeReference(const LifetimeReference<T>&) = default;
            LifetimeReference(LifetimeReference<T>&&) = default;

            auto get() const -> T*
            {
                return static_cast<T*>(LifetimeTrackerManager::get(handle));
            }

            auto reset() -> void
            {
                handle.reset();
            }

            auto assign(const T* rhs) -> void
            {
                // Only works if T inherits from LifetimeTracker<T>
                if (rhs)
                    handle = rhs->getLTHandle();
                else
                    this->reset();
            }

            template <typename NT, typename U = T, typename = decltype(static_cast<NT*>(std::declval<U*>()))>
            auto as() const -> LifetimeReference<NT>
            {
                return LifetimeReference<NT>(handle);
            }

            auto operator*() const -> T&
            {
                return *this->get();
            }

            auto operator->() const -> T*
            {
                return this->get();
            }

            auto operator=(const LifetimeReference& rhs) -> LifetimeReference&
            {
                handle = rhs.handle;
                return *this;
            }

            auto operator=(const LifetimeHandle& rhs) -> LifetimeReference&
            {
                handle = rhs;
                return *this;
            }

            auto operator=(const T* rhs) -> LifetimeReference&
            {
                this->assign(rhs);
                return *this;
            }

            auto operator==(const LifetimeReference<T>& rhs) const -> bool
            {
                return handle == rhs.handle;
            }

            auto operator!=(const LifetimeReference<T>& rhs) const -> bool
            {
                return !this->operator==(rhs);
            }

            explicit operator bool() const
            {
                return this->get();
            }

            template <typename NT, typename U = T, typename = typename std::enable_if<std::is_base_of<NT, U>::value>::type>
            operator LifetimeReference<NT>() const
            {
                return LifetimeReference<NT>(handle);
            }

        public:
            LifetimeHandle handle;
    };
}

#endif
