#include "gamelib/core/update/UpdateSystem.hpp"
#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    UpdateSystem::Handle UpdateSystem::add(UpdateComponent* obj, UpdateHookType hook)
    {
        assert(obj != nullptr && "UpdateComponent is null");

        auto h = _objs[hook].acquire();
        auto& o = _objs[hook][h];
        o.obj = obj;
        o.nextupdate = obj->interval;
        o.elapsed = 0;
        LOG_DEBUG("Added UpdateComponent to UpdateSystem");
        return h;
    }

    void UpdateSystem::remove(Handle handle, UpdateHookType hook)
    {
        _objs[hook].destroy(handle);
        LOG_DEBUG("Removed UpdateComponent from UpdateSystem");
    }

    void UpdateSystem::destroy()
    {
        for (auto& i : _objs)
            i.clear();
        LOG_DEBUG_WARN("UpdateSystem destroyed");
    }

    void UpdateSystem::update(float elapsed)
    {
        for (auto& h : _objs)
            for (auto& i : h)
            {
                --i.nextupdate;
                i.elapsed += elapsed;

                if (i.nextupdate == 0)
                {
                    // This must come first, because the object might delete itself
                    auto frametime = i.elapsed;
                    i.nextupdate = i.obj->interval;
                    i.elapsed = 0;
                    i.obj->update(frametime);
                }
            }
    }
}
