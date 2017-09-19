#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Entity* getEntity(Entity::Handle handle)
    {
        return getSubsystem<EntityManager>()->get(handle);
    }


    Entity::Entity() :
        _entmgr(nullptr),
        _quitting(false)
    { }

    Entity::Entity(const std::string& name) :
        _entmgr(nullptr),
        _name(name),
        _quitting(false)
    { }

    Entity::Entity(Entity&& other) :
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

    bool Entity::loadFromJson(const Json::Value& node)
    {
        _name = node.get("name", "").asString();
        gamelib::loadFromJson(node["transform"], _transform);

        // NOTE: Components aren't loaded this way
        // TODO: maybe reload existing components this way

        return true;
    }

    void Entity::writeToJson(Json::Value& node)
    {
        writeToJson(node, [](Json::Value& compnode, Component& comp) {
                comp.writeToJson(compnode);
            });
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

        if (_entmgr)
            comp->_ent = _handle;
        else
            comp->_entptr = this;

        if (!comp->_init())
        {
            comp->_ent = Handle();
            comp->_entptr = nullptr;
            LOG_ERROR("Failed to add component ", comp->getName(), " to entity ", _name);
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
                if (!_quitting)
                    _components.erase(it);
                else
                    it->reset();
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

    Component* Entity::find(const std::string& name) const
    {
        for (auto it = _components.begin(), end = _components.end(); it != end; ++it)
            if ((*it)->getName() == name)
                return it->get();
        return nullptr;
    }

    size_t Entity::size() const
    {
        return _components.size();
    }

    void Entity::clear()
    {
        _quit();
        _transform.reset();
    }

    void Entity::_quit()
    {
        // Set _quitting to true to prevent possible segfaults when a
        // component removes another component during his _quit().
        // remove() will not remove components from the list, when
        // _quitting is set, but reset the pointer instead.
        _quitting = true;
        for (auto& i : _components)
            if (i)
                i->_quit();
        _components.clear();
        _quitting = false;
    }

    void Entity::_refresh()
    {
        for (auto& i : _components)
            if (i)
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

    Entity& Entity::operator=(Entity&& other)
    {
        _entmgr = std::move(other._entmgr);
        _name = std::move(other._name);
        _quitting = std::move(other._quitting);
        _handle = std::move(other._handle);
        _transform = std::move(other._transform);
        _components = std::move(other._components);
        other._entmgr = nullptr;
        other._handle = Handle();
        return *this;
    }
}
