#include "gamelib/rendering/Layer.hpp"
#include "gamelib/rendering/Scene.hpp"

namespace gamelib
{
    Layer::Layer(int depth, float parallax, unsigned int flags) :
        flags(flags),
        _scene(nullptr),
        _parallax(parallax),
        _depth(depth)
    { };

    void Layer::setDepth(int depth)
    {
        if (depth != _depth)
        {
            _depth = depth;
            if (_scene)
                _scene->_dirty = true;
        }
    }

    int Layer::getDepth() const
    {
        return _depth;
    }

    void Layer::setParallax(float parallax)
    {
        if (_parallax != parallax)
        {
            _parallax = parallax;
            _scene->_dirty = true;
        }
    }

    float Layer::getParallax() const
    {
        return _parallax;
    }
}
