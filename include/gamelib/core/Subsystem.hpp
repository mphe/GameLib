#ifndef GAMELIB_SUBSYSTEM_HPP
#define GAMELIB_SUBSYSTEM_HPP

#include "gamelib/utils/log.hpp"

namespace gamelib
{
    // Adapted from https://stackoverflow.com/questions/13786888/check-if-member-exists-using-enable-if
    namespace detail
    {
        class general_ {};
        class special_ : public general_ {};

        template <typename T, typename = decltype(T::name)>
        void name_getter(special_)
        {
            LOG_WARN("Trying to access non-existing subsystem ", T::name);
        };

        template <typename T>
        void name_getter(general_)
        {
            LOG_WARN("Trying to access non-existing subsystem");
        };

        template <typename T>
        void print_name()
        {
            name_getter<T>(special_());
        }
    }


    template <typename T>
    class Subsystem
    {
        public:
            Subsystem()
            {
                if (!_active)
                    setActive();
            }

            virtual ~Subsystem()
            {
                if (_active == static_cast<T*>(this))
                    _active = nullptr;
            }

            auto setActive() -> void
            {
                _active = static_cast<T*>(this);
            }

            static auto getActive() -> T*
            {
                if (!_active)
                    detail::print_name<T>();
                return _active;
            }

        private:
            static T* _active;
    };


    template <typename T>
    T* getSubsystem()
    {
        return Subsystem<T>::getActive();
    }


    template <typename T>
    T* Subsystem<T>::_active = nullptr;
}

#endif
