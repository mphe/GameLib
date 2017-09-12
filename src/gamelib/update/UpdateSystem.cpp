#include "gamelib/update/UpdateSystem.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    constexpr const char* UpdateSystem::name;

    UpdateSystem::Handle UpdateSystem::add(Updatable* obj, HookType hook)
    {
        assert(obj != nullptr && "Updatable is null");

        auto h = _objs[hook].acquire();
        auto& o = _objs[hook][h];
        o.obj = obj;
        o.nextupdate = 1;
        o.elapsed = 0;
        LOG_DEBUG("Added Updatable to UpdateSystem");
        return h;
    }

    void UpdateSystem::remove(Handle handle, HookType hook)
    {
        _objs[hook].destroy(handle);
        LOG_DEBUG("Removed Updatable from UpdateSystem");
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
                    i.obj->update(i.elapsed);
                    i.nextupdate = i.obj->interval;
                    i.elapsed = 0;
                }
            }
    }
}
