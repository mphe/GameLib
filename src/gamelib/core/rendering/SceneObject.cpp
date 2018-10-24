#include "gamelib/core/rendering/SceneObject.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/json.hpp"
#include "gamelib/utils/conversions.hpp"
#include <cassert>

namespace gamelib
{
    SceneObject::SceneObject(int depth, float parallax, unsigned int flags) :
        SceneData(depth, parallax, flags),
        Transformable(true, true, true)
    { };

    void SceneObject::setLayer(Layer::Handle layer)
    {
        _layer = layer;
        if (_scene && layer != _layer)
            _scene->_dirty = true;
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


    void SceneObject::render(sf::RenderTarget& target) const
    {
        render(target, sf::RenderStates::Default);
    }

    void SceneObject::render(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getMatrix();
        target.draw(_vertices, states);
    }


    void SceneObject::_updateBBox()
    {
        _bbox = convert(getMatrix().transformRect(_vertices.getBounds()));
        _markDirty();
    }


    bool SceneObject::loadFromJson(const Json::Value& node)
    {
        SceneData::loadFromJson(node);

        auto scene = _scene ? _scene : getSubsystem<Scene>();

        if (!scene)
            LOG_WARN("No Scene available -> Layer cannot be applied");
        else
        {
            auto layer = scene->getLayer(node["layer"].asString());
            if (!layer.isNull())
                setLayer(layer);
        }

        gamelib::loadFromJson(node["transform"], *static_cast<Transformable*>(this), false);

        return true;
    }

    void SceneObject::writeToJson(Json::Value& node)
    {
        SceneData::writeToJson(node);

        auto scene = _scene ? _scene : getSubsystem<Scene>();
        if (scene)
        {
            auto layer = scene->getLayer(getLayer());
            if (layer)
                node["layer"] = layer->getName();
        }

        gamelib::writeToJson(node["transform"], *static_cast<Transformable*>(this));
    }


    const math::AABBf& SceneObject::getBBox() const
    {
        return _bbox;
    }


    math::AABBf SceneObject::getLocalBounds() const
    {
        auto bounds = _vertices.getBounds();
        return math::AABBf(bounds.left, bounds.top, bounds.width, bounds.height);
    }

    const sf::VertexArray& SceneObject::getVertices() const
    {
        return _vertices;
    }

    size_t SceneObject::size() const
    {
        return _vertices.getVertexCount();
    }

    void SceneObject::_onChanged(const sf::Transform& old)
    {
        _updateBBox();
    }
}
