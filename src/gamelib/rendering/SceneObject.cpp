#include "gamelib/rendering/SceneObject.hpp"
#include "gamelib/rendering/Scene.hpp"

namespace gamelib
{
    SceneObject::SceneObject(int depth, unsigned int flags) :
        Layer(depth, 1, flags)
    { };

    void SceneObject::setLayer(Layer::Handle layer)
    {
        if (layer != _layer)
        {
            _layer = layer;
            if (_scene)
                _scene->_dirty = true;
        }
    }

    Layer::Handle SceneObject::getLayer() const
    {
        return _layer;
    }

    void SceneObject::unregister()
    {
        if (_scene)
            _scene->remove(this);
    }
}
