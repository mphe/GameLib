#include "gamelib/core/rendering/SceneObject.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/json.hpp"
#include <cassert>

namespace gamelib
{
    SceneObject::SceneObject(int depth, float parallax, unsigned int flags) :
        SceneData(depth, parallax, flags),
        Transformable(movable | rotatable | scalable),
        _scale(1, 1),
        _rotation(0)
    { };

    void SceneObject::setLayer(Layer::Handle layer)
    {
        if (!_scene)
            _layer = layer;
        else if (layer != _layer)
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

    void SceneObject::render(sf::RenderTarget& target) const
    {
        render(target, sf::RenderStates());
    }

    void SceneObject::render(sf::RenderTarget& target, const sf::RenderStates& states_) const
    {
        sf::RenderStates states(states_);
        states.transform.combine(getTransform());
        target.draw(_vertices, states);
    }


    void SceneObject::_updateBBox()
    {
        auto bounds = getTransform().transformRect(_vertices.getBounds());
        _bbox = math::AABBf(bounds.left, bounds.top, bounds.width, bounds.height);
        markDirty();
    }


    bool SceneObject::loadFromJson(const Json::Value& node)
    {
        SceneData::loadFromJson(node);

        auto scene = _scene ? _scene : getSubsystem<Scene>();
        assert(scene && "There must be a Scene to load this component");

        auto layer = scene->getLayer(node["layer"].asString());
        if (!layer.isNull())
            setLayer(layer);

        gamelib::loadFromJson(node["transform"], *static_cast<Transformable*>(this));
        gamelib::loadFromJson(node["origin"], _origin);

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
        gamelib::writeToJson(node["origin"], _origin);
    }


    void SceneObject::move(const math::Vec2f& rel)
    {
        _bbox.pos += rel;
        _pos += rel;
        _updateBBox();
        Transformable::move(rel);
    }

    void SceneObject::scale(const math::Vec2f& scale)
    {
        _bbox.size *= scale;
        _scale *= scale;
        _updateBBox();
        Transformable::scale(scale);
    }

    void SceneObject::rotate(float angle)
    {
        _rotation += angle;
        _updateBBox();
        Transformable::rotate(angle);
    }

    void SceneObject::setOrigin(const math::Point2f& origin)
    {
        _origin = origin;
    }

    const math::Point2f& SceneObject::getOrigin() const
    {
        return _origin;
    }

    const math::Point2f& SceneObject::getPosition() const
    {
        return _pos;
    }

    const math::Vec2f& SceneObject::getScale() const
    {
        return _scale;
    }

    float SceneObject::getRotation() const
    {
        return _rotation;
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

    sf::Transform SceneObject::getTransform() const
    {
        sf::Transform trans;
        trans.translate(_pos.x - _origin.x, _pos.y - _origin.y);
        trans.rotate(_rotation, _origin.x, _origin.y);
        trans.scale(_scale.x, _scale.y, _origin.x, _origin.y);
        return trans;
    }
}
