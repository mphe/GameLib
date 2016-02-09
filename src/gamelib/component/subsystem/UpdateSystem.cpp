#include "gamelib/component/subsystem/UpdateSystem.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/component/UpdateComponent.hpp"
#include <algorithm>

namespace gamelib
{
    UpdateSystem::UpdateSystem() : _remove(0)
    { }

    void UpdateSystem::add(UpdateComponent* comp)
    {
        _comps.push_back(comp);
        if (!comp->init(this))
        {
            LOG_DEBUG_ERROR("Failed to initialize UpdateComponent (", comp->getID(), ") at 0x", comp, " -> Skip");
            _comps.pop_back();
        }
    }

    void UpdateSystem::remove(UpdateComponent* comp)
    {
        auto it = std::find(_comps.begin(), _comps.end(), comp);
        if (it != _comps.end() && !it->remove)
        {
            it->remove = true;
            ++_remove;

            // Don't use virtual functions of comp here because it could crash in destructors.
            // TODO: Make this not crash ^
            LOG_DEBUG_WARN("UpdateComponent at 0x", comp, " marked for removal");
        }
    }

    void UpdateSystem::destroy()
    {
        _comps.clear();
        _remove = 0;
        LOG_DEBUG_WARN("UpdateSystem destroyed");
    }

    void UpdateSystem::update(float fps)
    {
        // Save the size at the begin to avoid updating components that were added inside the loop
        for (size_t i = 0, s = _comps.size(); i < s; ++i)
            if (!_comps[i].remove)
                _comps[i].comp->update(fps);

        _comps.erase(std::remove_if(_comps.begin(), _comps.end(), [](Data& c) {
            if (c.remove)
                LOG_DEBUG_WARN("UpdateComponent (", c.comp->getID(), ") at 0x", c.comp, " removed");
            return c.remove;
        }), _comps.end());

        _remove = 0;
    }

    size_t UpdateSystem::size() const
    {
        return _comps.size() - _remove;
    }
}
