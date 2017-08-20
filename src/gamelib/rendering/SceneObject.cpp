#include "gamelib/rendering/SceneObject.hpp"
#include "gamelib/rendering/Scene.hpp"
#include "gamelib/rendering/flags.hpp"
#include <cassert>

namespace gamelib
{
    SceneObject::SceneObject(int depth, unsigned int flags) :
        Layer(depth, 1, flags)
    { };

    void SceneObject::setLayer(Layer::Handle layer)
    {
        assert(_scene && "Scene is null");

        if (layer != _layer)
        {
            _layer = layer;
            _scene->_dirty = true;
        }
    }

    Layer::Handle SceneObject::getLayer() const
    {
        return _layer;
    }

    unsigned int SceneObject::getAllFlags() const
    {
        assert(_scene && "Scene is null");
        Layer* layer = _scene->getLayer(_layer);
        return flags | _scene->flags | (layer ? layer->flags : 0);
    }

    bool SceneObject::isVisible() const
    {
        auto allflags = getAllFlags();
        return !(allflags & render_invisible ||
                (allflags & render_hidden && !(allflags & render_drawhidden)));
    }

    void SceneObject::unregister()
    {
        if (_scene)
            _scene->remove(this);
    }
}
