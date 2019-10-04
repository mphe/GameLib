#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/utils/log.hpp"
#include <cassert>

namespace gamelib
{
    EntityHandle getEntity(Entity::Handle handle)
    {
        return handle;
    }



    Entity::Entity() :
        Entity("")
    { }

    Entity::Entity(const std::string& name) :
        flags(0),
        _name(name),
        _clearing(false),
        _parent(nullptr)
    { }

    // Entity::Entity(Entity&& other)
    // {
    //     *this = std::move(other);
    // }

    Entity::~Entity()
    {
        _quit();
        LOG_DEBUG("Entity destroyed: ", getName());
    }


//     Entity& Entity::operator=(Entity&& other)
//     {
//         // This is completely broken and hard to fix with planned changes
//         // TODO: remove move constructor
// #warning "remove move constructor"
//
//         other.getParent()->addChild(this);
//
//         for (auto& i : other.getChildren())
//             addChild(i);
//
//         flags = other.flags;
//         _entmgr = other._entmgr;
//         _clearing = other._clearing;
//         _handle = other._handle;
//         _name = std::move(other._name);
//         _transform = std::move(other._transform);
//         _components = std::move(other._components);
//
//         other._parent = nullptr;
//         other._entmgr = nullptr;
//         other._handle.reset();
//
//         return *this;
//     }


    auto Entity::addChild(EntityPtr ent) -> EntityReference
    {
        { // Sanity checks
            if (!ent)
            {
                LOG_ERROR("Can't add child entity: nullptr");
                return nullptr;
            }

            if (ent.get() == this)
            {
                LOG_ERROR("Can't add child entity: child == this");
                return nullptr;
            }

            // Can't set a parent as child
            if (isChildOf(ent.get()))
            {
                LOG_ERROR("Can't add child entity: child is parent");
                return nullptr;
            }

            if (ent->getParent())
            {
                LOG_ERROR("New child entity should not have a parent. This should never happen!");
                return nullptr;
            }
        }

        Entity* tmp = ent.get();    // ent is no longer valid after moving
        _children.push_back(std::move(ent));
        tmp->_parent = this;
        getTransform().add(&tmp->getTransform());

        return tmp;
    }

    auto Entity::popChild(EntityReference ent) -> EntityPtr
    {
        for (size_t i = 0; i < _children.size(); ++i)
            if (_children[i].get() == ent.get())
                return popChild(i);
        return nullptr;
    }

    auto Entity::popChild(size_t index) -> EntityPtr
    {
        if (index >= _children.size())
            return nullptr;

        EntityPtr ptr = std::move(_children[index]);
        ptr->_parent = nullptr;
        _children.erase(_children.begin() + index);
        getTransform().remove(&ptr->getTransform());
        return ptr;
    }

    auto Entity::reparent(EntityReference ent) -> EntityReference
    {
        { // Sanity checks
            if (!ent)
            {
                LOG_ERROR("Can't change parent: parent is nullptr");
                return nullptr;
            }

            if (ent.get() == this)
            {
                LOG_ERROR("Can't change parent: parent == this");
                return nullptr;
            }

            // Can't set a parent as child
            if (ent->isChildOf(this))
            {
                LOG_ERROR("Can't change parent: parent is child");
                return nullptr;
            }

            if (!getParent())
            {
                LOG_ERROR("Can't change parent: root node");
                return nullptr;
            }
        }

        ent->addChild(getParent()->popChild(this));
        return this;
    }

    auto Entity::getChildren() const -> const std::vector<EntityPtr>&
    {
        return _children;
    }

    auto Entity::getParent() const -> EntityReference
    {
        return _parent;
    }

    auto Entity::isChildOf(EntityReference ent) const -> bool
    {
        // Always child of root
        if (!ent)
            return true;

        for (EntityReference parent = getParent(); parent; parent = parent->getParent())
            if (parent == ent)
                return true;
        return false;
    }


    void Entity::destroy()
    {
        _quit();

        if (getParent())
            getParent()->popChild(this); // Suicide - Don't run any code after this
        else
            LOG_WARN("Can't free root entity automatically, but clear all children and components");
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
                it->ptr->_entptr.reset();
                it->ptr->quit();
                it->ptr.reset();

                if (!_clearing)
                    _components.erase(it);
                return;
            }
    }

    auto Entity::hasComponent(Component* comp) const -> bool
    {
        for (auto& i : _components)
            if (i.ptr.get() == comp)
                return true;
        return false;
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
        // Set _clearing to true to prevent possible segfaults when a
        // component removes another component during his _quit().
        // remove() will not remove components from the list, when
        // _clearing is set, but reset the pointer instead.

        if (_clearing)
            return;

        _clearing = true;
        for (auto& i : _components)
            if (i.ptr)
                i.ptr->quit();
        _components.clear();
        _clearing = false;
    }

    void Entity::_quit()
    {
        _children.clear();
        clear();
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
}
