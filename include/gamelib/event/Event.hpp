#ifndef GAMELIB_EVENT_HPP
#define GAMELIB_EVENT_HPP

#include <memory>
#include "../Identifiable.hpp"

namespace gamelib
{
    typedef ID EventID;
    constexpr EventID invalidEvent = invalidID;

    typedef IdentifiableBase BaseEvent; // Event Interface

    typedef std::shared_ptr<BaseEvent> EventPtr;

    // Wrapper (see Identifiable.hpp)
    template <int ID>
    using Event = Identifiable<ID>;
}

#endif
