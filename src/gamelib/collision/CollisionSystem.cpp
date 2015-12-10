#include "gamelib/collision/CollisionSystem.hpp"
#include <cassert>
#include <algorithm>

// TODO: Could be optimized for less code duplication.

namespace gamelib
{
    void CollisionSystem::add(Collidable* col)
    {
        _objs[col->getID()].push_back(col);
    }

    void CollisionSystem::remove(Collidable* col)
    {
        auto it = _objs.find(col->getID());
        if (it != _objs.end())
        {
            it->second.erase(std::find(it->second.begin(), it->second.end(), col));
            if (it->second.empty())
                _objs.erase(it);
        }
    }

    void CollisionSystem::destroy()
    {
        _objs.clear();
    }

    size_t CollisionSystem::size() const
    {
        size_t c = 0;
        for (auto& i : _objs)
            c += i.second.size();
        return c;
    }

    Collidable* CollisionSystem::contains(const geometry::Vector2<float>& point) const
    {
        for (auto& i : _objs)
            for (auto o : i.second)
                if (o->contains(point))
                    return o;
        return NULL;
    }

    Collidable* CollisionSystem::intersects(const geometry::AABB<float>& rect) const
    {
        for (auto& i : _objs)
            for (auto o : i.second)
                if (o->intersects(rect))
                    return o;
        return NULL;
    }

    Collidable* CollisionSystem::contains(const geometry::Vector2<float>& point, ID id) const
    {
        auto it = _objs.find(id);
        if (it != _objs.end())
            for (auto o : it->second)
                if (o->contains(point))
                    return o;
        return NULL;
    }

    Collidable* CollisionSystem::intersects(const geometry::AABB<float>& rect, ID id) const
    {
        auto it = _objs.find(id);
        if (it != _objs.end())
            for (auto o : it->second)
                if (o->intersects(rect))
                    return o;
        return NULL;
    }
}
