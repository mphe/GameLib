#ifndef GAMELIB_UPDATE_SYSTEM_HPP
#define GAMELIB_UPDATE_SYSTEM_HPP

#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "Updatable.hpp"

/*
 * Updates all registered Updatables in the given intervals.
 * The frametime between individual updates is saved and passed to the object
 * when it's time to update again.
 */

namespace gamelib
{
    class UpdateSystem : public Updatable, public Subsystem<UpdateSystem>
    {
        public:
            enum HookType
            {
                PrePreFrame,
                PreFrame,
                Frame,
                PostFrame,
                PostPostFrame,
                NumFrameHooks
            };

            typedef SlotKeyShort Handle;

            constexpr static const char* name = "UpdateSystem";

        public:
            auto add(Updatable* obj, HookType hook)   -> Handle;
            auto remove(Handle handle, HookType hook) -> void;
            auto destroy()                            -> void;

            auto update(float elapsed) -> void;

        private:
            struct Data
            {
                Updatable* obj;
                int nextupdate;
                float elapsed;
            };

        private:
            SlotMapShort<Data> _objs[NumFrameHooks];
    };
}

#endif
