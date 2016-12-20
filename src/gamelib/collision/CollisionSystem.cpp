#include "gamelib/collision/CollisionSystem.hpp"
#include <algorithm>

namespace gamelib
{
    void CollisionSystem::add(Collidable* col)
    {
        _objs.push_back(col);
    }

    void CollisionSystem::remove(Collidable* col)
    {
        _objs.erase(std::find(_objs.begin(), _objs.end(), col));
    }

    void CollisionSystem::destroy()
    {
        _objs.clear();
    }

    size_t CollisionSystem::size() const
    {
        return _objs.size();
    }

    Collidable* CollisionSystem::trace(const math::Point2f& point, unsigned int flags) const
    {
        for (auto& i : _objs)
            if ((!flags || i->flags & flags) && i->intersect(point))
                return i;
        return nullptr;
    }

    TraceResult CollisionSystem::trace(const math::Line2f& line, unsigned int flags) const
    {
        TraceResult nearest;
        for (auto& i : _objs)
        {
            if ((!flags || i->flags & flags))
            {
                auto isec = i->intersect(line);
                if (isec.type == math::LinexLine)
                    std::swap(isec.near, isec.far);

                if (!nearest || isec.near < nearest.isec.near)
                {
                    nearest.obj = i;
                    nearest.isec = isec;
                }
            }
        }
        return nearest;
    }


    TraceResult::TraceResult() :
        obj(nullptr)
    { }

    TraceResult::operator bool() const
    {
        return isec;
    }
}
