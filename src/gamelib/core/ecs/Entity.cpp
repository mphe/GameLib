#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Entity* getEntity(Entity::Handle handle)
    {
        return getSubsystem<EntityManager>()->get(handle);
    }

    Entity* findEntity(const std::string& name)
    {
        return getSubsystem<EntityManager>()->find(name);
    }

    auto findEntityHandle(const std::string& name) -> Entity::Handle
    {
        auto ent = findEntity(name);
        if (ent)
            return ent->getHandle();
        else
            return Entity::Handle();
    }



    Entity::Entity() :
        Entity("")
    { }

    Entity::Entity(const std::string& name) :
        flags(0),
        _entmgr(nullptr),
        _name(name),
        _quitting(false)
    { }

    Entity::Entity(Entity&& other) :
        flags(other.flags),
        _entmgr(std::move(other._entmgr)),
        _handle(std::move(other._handle)),
        _name(std::move(other._name)),
        _transform(std::move(other._transform)),
        _quitting(std::move(other._quitting)),
        _components(std::move(other._components))
    {
        other._entmgr = nullptr;
        other._handle = Handle();
    }

    Entity::~Entity()
    {
        destroy();
    }


    void Entity::destroy()
    {
        if (_entmgr)
            _entmgr->destroy(_handle);
        else
            clear();
    }

    Entity::Handle Entity::getHandle() const
    {
        return _handle;
    }

    const std::string& Entity::getName() const
    {
        return _name;
    }

    const GroupTransform& Entity::getTransform() const
    {
        return _transform;
    }

    GroupTransform& Entity::getTransform()
    {
        return _transform;
    }

    Component* Entity::add(ComponentPtr comp)
    {
        assert(comp && "Component is nullptr");

        if (!comp->init())
        {
            LOG_ERROR("Failed to add component ", comp->getName(), " to entity ", _name);
            return nullptr;
        }

        if (_entmgr)
            comp->_ent = _handle;
        else
            comp->_entptr = this;

        if (comp->getTransform())
            getTransform().add(comp->getTransform());

        // Find highest id of these component types
        unsigned int id = 0;
        for (auto& i : _components)
            if (i.ptr->getName() == comp->getName())
                id = std::max(id, i.id);
        ++id;

        auto ptr = comp.get();
        _components.push_back({ id, std::move(comp) });
        _refresh(ComponentAdded, ptr);
        return ptr;
    }

    void Entity::remove(Component* comp)
    {
        for (auto it = _components.begin(), end = _components.end(); it != end; ++it)
            if (it->ptr.get() == comp)
            {
                _refresh(ComponentRemoved, it->ptr.get());

                if (comp->getTransform())
                    getTransform().remove(comp->getTransform());

                it->ptr->_ent.reset();
                it->ptr->_entptr = nullptr;
                it->ptr->quit();
                it->ptr.reset();

                if (!_quitting)
                    _components.erase(it);
                return;
            }
    }

    Component* Entity::find(ID type) const
    {
        Component* ptr = nullptr;
        findAll(type, [&ptr](Component* comp) {
                ptr = comp;
                return true; // Break after first found
            });
        return ptr;
    }

    Component* Entity::find(const std::string& name) const
    {
        Component* ptr = nullptr;
        findAll(name, [&ptr](Component* comp) {
                ptr = comp;
                return true; // Break after first found
            });
        return ptr;
    }

    size_t Entity::size() const
    {
        return _components.size();
    }

    void Entity::clear()
    {
        _quit();
        _transform.reset();
        flags = 0;
    }

    void Entity::_quit()
    {
        // Set _quitting to true to prevent possible segfaults when a
        // component removes another component during his _quit().
        // remove() will not remove components from the list, when
        // _quitting is set, but reset the pointer instead.
        _quitting = true;
        for (auto& i : _components)
            if (i.ptr)
                i.ptr->quit();
        _components.clear();
        _quitting = false;
    }

    void Entity::_refresh(RefreshType type, Component* comp)
    {
        for (auto& i : _components)
            if (i.ptr)
                i.ptr->_refresh(type, comp);
    }


    Entity::ComponentList::const_iterator Entity::begin() const
    {
        return _components.begin();
    }

    Entity::ComponentList::const_iterator Entity::end() const
    {
        return _components.end();
    }


    Entity& Entity::operator=(Entity&& other)
    {
        flags = other.flags;
        _entmgr = other._entmgr;
        _quitting = other._quitting;
        _handle = other._handle;
        _name = std::move(other._name);
        _transform = std::move(other._transform);
        _components = std::move(other._components);
        other._entmgr = nullptr;
        other._handle.reset();
        return *this;
    }
}
