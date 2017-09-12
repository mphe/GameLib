#include "gamelib/geometry/CollisionSystem.hpp"
#include <algorithm>

namespace gamelib
{
    constexpr const char* CollisionSystem::name;

    void CollisionSystem::add(Collidable* col)
    {
        _objs.push_back(col);
    }

    void CollisionSystem::remove(Collidable* col)
    {
        auto it = std::find(_objs.begin(), _objs.end(), col);
        if (it != _objs.end())
            _objs.erase(it);
    }

    void CollisionSystem::destroy()
    {
        _objs.clear();
    }

    size_t CollisionSystem::size() const
    {
        return _objs.size();
    }

    Collidable* CollisionSystem::find(const math::Point2f& point, unsigned int flags) const
    {
        for (auto it = _objs.rbegin(), end = _objs.rend(); it != end; ++it)
        {
            Collidable* i = (*it);
            if ((!flags || i->flags & flags) && i->intersect(point))
                return i;
        }
        return nullptr;
    }

    TraceResult CollisionSystem::trace(const math::Line2f& line, unsigned int flags) const
    {
        TraceResult nearest;
        for (auto it = _objs.rbegin(), end = _objs.rend(); it != end; ++it)
        {
            Collidable* i = (*it);
            if ((!flags || i->flags & flags))
            {
                auto isec = i->intersect(line);
                if (isec.type == math::LinexLine)
                    std::swap(isec.near, isec.far);

                if (isec && (!nearest || isec.near < nearest.isec.near))
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
