#ifndef GAMELIB_SUBSYSTEM_HPP
#define GAMELIB_SUBSYSTEM_HPP

#include "utils/log.hpp"

namespace gamelib
{
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
                    LOG_WARN("Trying to access non-existing subsystem");
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
