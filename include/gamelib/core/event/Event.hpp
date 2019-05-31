#ifndef GAMELIB_EVENT_HPP
#define GAMELIB_EVENT_HPP

#include <memory>
#include "gamelib/utils/Identifier.hpp"

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
    // The first template parameter is the event ID, the second is the
    // derived event class. The second one is optional and provides a static
    // create() function that returns an EventPtr to the derived class.
    template <ID id, typename T = void>
    class Event : public Identifier<id, BaseEvent>
    {
        public:
            typedef std::shared_ptr<T> Pointer;

        public:
            template <typename... Args, typename TT = T, typename = typename std::enable_if<!std::is_void<TT>::value>::type>
            static EventPtr create(Args&&... args)
            {
                return EventPtr(new T(std::forward<Args>(args)...));
            }
    };
}

#endif
