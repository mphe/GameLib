#include "gamelib/core/geometry/GroupTransform.hpp"

namespace gamelib
{
    GroupTransform::GroupTransform() :
        _dirty(false)
    { }

    void GroupTransform::add(Transformable* trans)
    {
        _objs.push_back(trans);
        trans->_setParent(this);
        _dirty = true;
    }

    void GroupTransform::remove(Transformable* trans)
    {
        auto it = std::find(_objs.begin(), _objs.end(), trans);
        if (it != _objs.end())
        {
            if (_objs.size() > 1)
                std::swap(*it, _objs.back());
            _objs.pop_back();
            trans->_setParent(nullptr);
            _dirty = true;
        }
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

    void GroupTransform::_onChanged(const sf::Transform& old)
    {
        for (auto& i : _objs)
            i->_updateMatrix();
    }
}
