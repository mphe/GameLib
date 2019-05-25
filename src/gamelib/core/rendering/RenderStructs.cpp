#include "gamelib/core/rendering/RenderStructs.hpp"
#include "gamelib/core/rendering/flags.hpp"

namespace gamelib
{
    RenderLayer::RenderLayer() :
        depth(0)
    { }

    Mesh::Mesh() :
        size(0)
    { }

    SceneNode::SceneNode() :
        depth(0),
        owner(nullptr),
        _bboxdirty(false)
    { }


	RenderOptions::RenderOptions() :
        flags(0),
        parallax(1),
        shader(nullptr)
    { }

    auto RenderOptions::inherited(const RenderOptions& parent) const -> RenderOptions
    {
        return RenderOptions(*this).inherit(parent);
    }

    auto RenderOptions::inherit(const RenderOptions& parent) -> RenderOptions&
    {
        flags |= parent.flags;
        parallax *= parent.parallax;

        if (!shader)
            shader = parent.shader;
        if (!texture)
            texture = parent.texture;

        // NOTE: see in practice if this behavior makes sense
        if (blendMode == sf::RenderStates::Default.blendMode)
            blendMode = parent.blendMode;

        return *this;
    }

    auto RenderOptions::isVisible() const -> bool
    {
        return !(flags & render_invisible ||
                (flags & render_hidden && !(flags & render_drawhidden)));
    }


    const RenderOptions RenderOptions::defaultOptions;
}
