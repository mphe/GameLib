#include "gamelib/core/geometry/GroupTransform.hpp"

// TODO: adapt position for individual elements, when scaling/rotating

namespace gamelib
{
    GroupTransform::GroupTransform() :
        GroupTransform(math::Point2f())
    { }

    GroupTransform::GroupTransform(float x, float y) :
        GroupTransform(math::Point2f(x, y))
    { }

    GroupTransform::GroupTransform(const math::Point2f& pos) :
        _pos(pos),
        _scale(1, 1),
        _rotation(0)
    { }

    void GroupTransform::add(Transformable* trans, bool relative)
    {
        _objs.push_back(trans);
        trans->_parent = this;
        if (relative)
            *trans += *this;
        _dirty = true;
    }

    void GroupTransform::remove(Transformable* trans, bool relative)
    {
        auto it = std::find(_objs.begin(), _objs.end(), trans);
        if (it != _objs.end())
        {
            if (_objs.size() > 1)
                std::swap(*it, _objs.back());
            _objs.pop_back();
            trans->_parent = nullptr;
            if (relative)
                *trans -= *this;
            _dirty = true;
        }
    }

    void GroupTransform::move(const math::Vec2f& rel)
    {
        for (auto& i : _objs)
            i->move(rel);
        _pos += rel;
        _dirty = true;
        Transformable::move(rel);
    }

    void GroupTransform::scale(const math::Vec2f& scale_)
    {
        for (auto& i : _objs)
            i->scale(scale_);
        _scale *= scale_;
        _dirty = true;
        Transformable::scale(scale_);
    }

    void GroupTransform::rotate(float angle)
    {
        for (auto& i : _objs)
            i->rotate(angle);
        _rotation += angle;
        _dirty = true;
        Transformable::rotate(angle);
    }


    const math::Point2f& GroupTransform::getPosition() const
    {
        return _pos;
    }

    const math::Vec2f& GroupTransform::getScale() const
    {
        return _scale;
    }

    float GroupTransform::getRotation() const
    {
        return _rotation;
    }

    const math::AABBf& GroupTransform::getBBox() const
    {
        if (_dirty)
        {
            math::AABBf bbox;
            for (auto& i : _objs)
                bbox.combine(i->getBBox());
            _bbox = bbox;
            _dirty = false;
        }
        return _bbox;
    }

    const std::vector<Transformable*>& GroupTransform::getChildren() const
    {
        return _objs;
    }
}
