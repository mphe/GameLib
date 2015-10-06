#ifndef SYSTEM_INTERFACE_HPP
#define SYSTEM_INTERFACE_HPP

namespace engine
{
    class Game;
    class EventManager;

    class ISystem
    {
        public:
            virtual ~ISystem() {}
            virtual Game* getGame() const = 0;
            virtual EventManager* getEventManager() const = 0;
    };
}

#endif
