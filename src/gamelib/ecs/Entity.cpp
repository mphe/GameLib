#include "gamelib/ecs/Entity.hpp"
#include "gamelib/ecs/EntityManager.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Entity::Entity() :
        _entmgr(nullptr)
    { }

    Entity::Entity(const std::string& name) :
        _entmgr(nullptr),
        _name(name)
    { }

    Entity::~Entity()
    {
        destroy();
    }


    void Entity::destroy()
    {
        if (_entmgr)
            _entmgr->destroy(_handle);
        else
            _quit();
    }


    Entity::Handle Entity::getHandle() const
    {
        return _handle;
    }

    const std::string& Entity::getName() const
    {
        return _name;
    }

    Component* Entity::add(ComponentPtr comp)
    {
        assert(comp && "Component is nullptr");

        comp->_ent = this;
        if (!comp->_init())
        {
            comp->_ent = nullptr;
            return nullptr;
        }

        _components.push_back(std::move(comp));
        _refresh();
        return _components.back().get();
    }

    void Entity::remove(Component* comp)
    {
        for (auto it = _components.begin(), end = _components.end(); it != end; ++it)
            if (it->get() == comp)
            {
                (*it)->_quit();
                _components.erase(it);
                _refresh();
                break;
            }
    }

    Component* Entity::find(ID type) const
    {
        for (auto it = _components.begin(), end = _components.end(); it != end; ++it)
            if ((*it)->getID() == type)
                return it->get();
        return nullptr;
    }

    size_t Entity::size() const
    {
        return _components.size();
    }

    void Entity::_quit()
    {
        for (auto& i : _components)
            i->_quit();
        _components.clear();

        LOG_DEBUG("Entity destroyed");
    }

    void Entity::_refresh()
    {
        for (auto& i : _components)
            i->_refresh();
    }


    Entity::ComponentList::const_iterator Entity::begin() const
    {
        return _components.begin();
    }

    Entity::ComponentList::iterator Entity::begin()
    {
        return _components.begin();
    }

    Entity::ComponentList::const_iterator Entity::end() const
    {
        return _components.end();
    }

    Entity::ComponentList::iterator Entity::end()
    {
        return _components.end();
    }
}
