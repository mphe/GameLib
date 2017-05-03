#include "gamelib/rendering/SceneObject.hpp"
#include "gamelib/rendering/Scene.hpp"

namespace gamelib
{
    SceneObject::SceneObject(int depth, unsigned int flags) :
        _flags(flags), _depth(depth), _scene(nullptr)
    {};

    void SceneObject::setDepth(int depth)
    {
        if (depth != _depth)
        {
            _depth = depth;
            if (_scene)
                _scene->_dirty = true;
        }
    }

    int SceneObject::getDepth() const
    {
        return _depth;
    }

    void SceneObject::unregister()
    {
        if (_scene)
            _scene->remove(this);
    }
}
