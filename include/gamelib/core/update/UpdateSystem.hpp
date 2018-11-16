#ifndef GAMELIB_UPDATE_SYSTEM_HPP
#define GAMELIB_UPDATE_SYSTEM_HPP

#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "Updatable.hpp"

/*
 * Updates all registered UpdateComponents in the given intervals.
 * The frametime between individual updates is saved and passed to the object
 * when it's time to update again.
 */

namespace gamelib
{
    class UpdateComponent;

    enum UpdateHookType
    {
        PrePreFrame,
        PreFrame,
        Frame,
        PostFrame,
        PostPostFrame,
        NumFrameHooks
    };

    constexpr const char* str_framehooks[] = {
        "PrePreFrame",
        "PreFrame",
        "Frame",
        "PostFrame",
        "PostPostFrame"
    };


    class UpdateSystem : public Updatable, public Subsystem<UpdateSystem>
    {
        public:
            typedef SlotKeyShort Handle;

            constexpr static const char* name = "UpdateSystem";

        public:
            auto add(UpdateComponent* obj, UpdateHookType hook)   -> Handle;
            auto remove(Handle handle, UpdateHookType hook) -> void;
            auto destroy()                            -> void;

            auto update(float elapsed) -> void;

        private:
            struct Data
            {
                UpdateComponent* obj;
                int nextupdate;
                float elapsed;
            };

        private:
            SlotMapShort<Data> _objs[NumFrameHooks];
    };
}

#endif
