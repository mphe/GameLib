#ifndef GAMELIB_EVENT_HPP
#define GAMELIB_EVENT_HPP

#include <memory>
#include "../Identifier.hpp"

namespace gamelib
{
    typedef ID EventID;
    constexpr EventID invalidEvent = invalidID;

    // typedef Identifiable BaseEvent; // Event Interface

    class BaseEvent : public Identifiable
    {
        public:
            virtual ~BaseEvent() {}

            // Returns a pointer T*.
            // Can be used to easily cast to the derived class.
            template <class T>
            const T* get() const
            {
                return static_cast<const T*>(this);
            }

            template <class T>
            T* get()
            {
                return static_cast<T*>(this);
            }
    };

    typedef std::shared_ptr<BaseEvent> EventPtr;

    // Wrapper (see Identifier.hpp)
    template <ID id>
    using Event = Identifier<id, BaseEvent>;
}

#endif
