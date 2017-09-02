#include "gamelib/update/UpdateSystem.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    UpdateSystem::Handle UpdateSystem::add(Updatable* obj)
    {
        assert(obj != nullptr && "Updatable is null");

        auto h = _objs.acquire();
        _objs[h].obj = obj;
        _objs[h].nextupdate = 1;
        _objs[h].elapsed = 0;
        LOG_DEBUG("Added Updatable to UpdateSystem");
        return h;
    }

    void UpdateSystem::remove(Handle handle)
    {
        _objs.destroy(handle);
        LOG_DEBUG("Removed Updatable from UpdateSystem");
    }

    void UpdateSystem::destroy()
    {
        _objs.clear();
        LOG_DEBUG_WARN("UpdateSystem destroyed");
    }

    void UpdateSystem::update(float elapsed)
    {
        for (auto& i : _objs)
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
