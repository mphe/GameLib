#ifndef GAMELIB_UPDATE_SYSTEM_HPP
#define GAMELIB_UPDATE_SYSTEM_HPP

#include "gamelib/utils/SlotMap.hpp"
#include "Updatable.hpp"

/*
 * Updates all registered Updatables in the given intervals.
 * The frametime between individual updates is saved and passed to the object
 * when it's time to update again.
 */

namespace gamelib
{
    class UpdateSystem : public Updatable
    {
        public:
            typedef SlotKeyShort Handle;

        public:
            auto add(Updatable* obj)   -> Handle;
            auto remove(Handle handle) -> void;
            auto destroy()             -> void;

            auto update(float elapsed) -> void;

        private:
            struct Data
            {
                Updatable* obj;
                int nextupdate;
                float elapsed;
            };

        private:
            SlotMapShort<Data> _objs;
    };
}

#endif
