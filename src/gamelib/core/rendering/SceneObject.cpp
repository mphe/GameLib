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

    void SceneObject::render(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(_vertices, states);
    }


    void SceneObject::_updateBBox()
    {
        auto bounds = getTransform().transformRect(_vertices.getBounds());
        _bbox = math::AABBf(bounds.left, bounds.top, bounds.width, bounds.height);
        markDirty();
    }

    void SceneObject::_updateTransform(bool force) const
    {
        if (!force && !_transDirty)
            return;

        _trans = sf::Transform::Identity;
        _trans.translate(_pos.x - _origin.x, _pos.y - _origin.y);
        _trans.rotate(_rotation, _origin.x, _origin.y);
        _trans.scale(_scale.x, _scale.y, _origin.x, _origin.y);
        _transDirty = false;
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
        _transDirty = true;

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
        _transDirty = true;
        Transformable::move(rel);
    }

    void SceneObject::scale(const math::Vec2f& scale)
    {
        _scale *= scale;
        _transDirty = true;
        _updateBBox();
        Transformable::scale(scale);
    }

    void SceneObject::rotate(float angle)
    {
        _rotation += angle;
        _transDirty = true;
        _updateBBox();
        Transformable::rotate(angle);
    }

    void SceneObject::setOrigin(const math::Point2f& origin)
    {
        _origin = origin;
        _transDirty = true;
        _updateBBox();
        markDirty();
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

    const sf::Transform& SceneObject::getTransform() const
    {
        _updateTransform();
        return _trans;
    }

    const sf::VertexArray& SceneObject::getVertices() const
    {
        return _vertices;
    }
}
