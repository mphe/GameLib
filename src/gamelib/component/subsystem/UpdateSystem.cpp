#include "gamelib/component/subsystem/UpdateSystem.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/component/UpdateComponent.hpp"
#include <algorithm>

namespace gamelib
{
    UpdateSystem::UpdateSystem() :
        _remove(0),
        _insert(0)
    { }

    void UpdateSystem::add(UpdateComponent* comp, int depth)
    {
        // The component is added to the list before calling init in case the component wants to change its index.
        _comps.push_back(Data(comp, depth));
        if (!comp->init(this))
        {
            _comps.pop_back();
            LOG_ERROR("Failed to initialize UpdateComponent (", comp->getID(), ") at 0x", comp, " -> Skip");
        }
        else
        {
            ++_insert;
        }
    }

    void UpdateSystem::remove(UpdateComponent* comp)
    {
        auto it = _find(comp);
        if (it != _comps.end() && !it->remove)
        {
            it->remove = true;
            ++_remove;

            // Don't use comp's virtual functions here because it could crash when called inside the destructor.
            LOG_DEBUG_WARN("UpdateComponent at 0x", comp, " marked for removal");
        }
    }

    void UpdateSystem::setDepth(UpdateComponent* comp, int depth)
    {
        auto it = _find(comp);
        if (it != _comps.end() && !it->remove && it->depth != depth)
        {
            it->depth = depth;
            ++_insert;
        }
    }

    void UpdateSystem::destroy()
    {
        _comps.clear();
        _remove = _insert = 0;
        LOG_DEBUG_WARN("UpdateSystem destroyed");
    }

    void UpdateSystem::update(float elapsed)
    {
        if (_insert)
        {
            LOG_DEBUG(_insert, " new/changed UpdateComponents -> sorting");

            // Components are sorted ascending by depth.
            // Removed components are moved to the back for easy erase()ing.
            // TODO: Use insert() if there are just a few new components.
            std::stable_sort(_comps.begin(), _comps.end());

            if (_remove)
                _comps.erase(_comps.end() - _remove, _comps.end());

            LOG_DEBUG(_remove, " UpdateComponents cleaned.");

            _remove = _insert = 0;
        }
        else if (_remove && !size())
        {
            _comps.clear();
            LOG_DEBUG(_remove, " UpdateComponents cleaned.");
            _remove = 0;
        }

        // Save the size at the begin to avoid updating components that were added inside the loop.
        // Use indexes instead of iterators because adding new elements could invalidate iterators.
        for (size_t i = 0, s = _comps.size(); i < s; ++i)
            if (!_comps[i].remove)
                _comps[i].comp->update(elapsed);
    }

    size_t UpdateSystem::size() const
    {
        return _comps.size() - _remove;
    }

    std::vector<UpdateSystem::Data>::iterator UpdateSystem::_find(UpdateComponent* comp)
    {
        return std::find_if(_comps.begin(), _comps.end(), [comp](const Data& d) {
                return d.comp == comp;
            });
    }


    UpdateSystem::Data::Data(UpdateComponent* c, int d) :
        comp(c),
        depth(d),
        remove(false)
    { }

    bool UpdateSystem::Data::operator==(const Data& d) const
    {
        return comp == d.comp && depth == d.depth && remove == d.remove;
    }

    bool UpdateSystem::Data::operator<(const Data& d) const
    {
        if (remove)
            return false;
        if (d.remove)
            return true;
        return depth < d.depth;
    }

    bool UpdateSystem::Data::operator>(const Data& d) const
    {
        if (d.remove)
            return false;
        if (remove)
            return true;
        return depth > d.depth;
    }
}
