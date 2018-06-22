#include "gamelib/core/rendering/Layer.hpp"

namespace gamelib
{
    Layer::Layer(int depth, float parallax, unsigned int flags) :
        Layer("unnamed", depth, parallax, flags)
    { };

    Layer::Layer(const std::string& name, int depth, float parallax, unsigned int flags) :
        SceneData(depth, parallax, flags),
        _name(name)
    { };

    const std::string& Layer::getName() const
    {
        return _name;
    }
}
