#ifndef GAMELIB_SYSTEM_HPP
#define GAMELIB_SYSTEM_HPP

/*
 * System provides getters for all common data structures in a game.
 *
 * A system class should inherit from System and implement the supported
 * getters.
 * The default getters return nullptr and log a warning that the requested
 * object is not supported.
 * 
 * A system can be set as active by calling setActive(). This makes it
 * accessible through the static function getActive().
 * This functionality allows accessing the current system without having to
 * pass pointers around.
 */

namespace gamelib
{
    class Game;
    class EventManager;
    class CollisionSystem;
    class ResourceManager;
    class Scene;

    class System
    {
        public:
            virtual ~System() {}
            virtual auto getScene()           -> Scene*;
            virtual auto getCollisionSystem() -> CollisionSystem*;
            virtual auto getEventManager()    -> EventManager*;
            virtual auto getResourceManager() -> ResourceManager*;
            virtual auto getGame()            -> Game*;

            auto setActive()        -> void;
            static auto getActive() -> System*;

        private:
            static System* _active;
    };
}

#endif
