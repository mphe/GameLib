#include "gamelib/rendering/Layer.hpp"

namespace gamelib
{
    Layer::Layer(int depth, float parallax, unsigned int flags) :
        SceneData(depth, parallax, flags),
        _id(0)
    { };

    size_t Layer::getUniqueID() const
    {
        return _id;
    }
}
