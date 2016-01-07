#ifndef GAMELIB_EVENT_HPP
#define GAMELIB_EVENT_HPP

#include <memory>
#include "../Identifier.hpp"

namespace gamelib
{
    typedef ID EventID;
    constexpr EventID invalidEvent = invalidID;

    typedef Identifiable BaseEvent; // Event Interface

    typedef std::shared_ptr<BaseEvent> EventPtr;

    // Wrapper (see Identifier.hpp)
    template <ID id>
    using Event = Identifier<id, BaseEvent>;
}

#endif
