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
        sf::Transform trans;
        trans.scale(_scale.x, _scale.y, _bbox.x, _bbox.y);
        trans.rotate(_rotation);
        trans.translate(_pos.x, _pos.y);

        sf::RenderStates states(states_);
        states.transform.combine(trans);

        target.draw(_vertices, states);
    }


    bool SceneObject::loadFromJson(const Json::Value& node)
    {
        SceneData::loadFromJson(node);

        auto scene = _scene ? _scene : getSubsystem<Scene>();
        auto layer = scene->getLayer(node.get("layer", (Json::UInt)-1).asUInt());
        if (!layer.isNull())
            setLayer(layer);

        gamelib::loadFromJson(node["transform"], *static_cast<Transformable*>(this));

        if (node.isMember("vertices"))
        {
            auto& vertices = node["vertices"];
            _vertices.resize(vertices.size());
            for (Json::ArrayIndex i = 0; i < vertices.size(); ++i)
            {
                gamelib::loadFromJson(vertices[i][0], _vertices[i].position);
                gamelib::loadFromJson(vertices[i][1], _vertices[i].texCoords);
            }
        }

        return true;
    }

    void SceneObject::writeToJson(Json::Value& node)
    {
        SceneData::writeToJson(node);

        auto scene = _scene ? _scene : getSubsystem<Scene>();
        auto layer = scene->getLayer(getLayer());
        if (layer)
            node["layer"] = (Json::UInt)layer->getUniqueID();

        gamelib::writeToJson(node["transform"], *static_cast<Transformable*>(this));

        auto& vertices = node["vertices"];
        vertices.resize(_vertices.getVertexCount());

        for (Json::ArrayIndex i = 0; i < _vertices.getVertexCount(); ++i)
        {
            auto& vertex = vertices[i];
            vertex.resize(2);
            gamelib::writeToJson(vertex[0], _vertices[i].position);
            gamelib::writeToJson(vertex[1], _vertices[i].texCoords);
        }
    }


    void SceneObject::move(const math::Vec2f& rel)
    {
        _bbox.pos += rel;
        _pos += rel;
    }

    void SceneObject::scale(const math::Vec2f& scale)
    {
        _bbox.size *= scale;
        _scale *= scale;
    }

    void SceneObject::rotate(float angle)
    {
        _rotation += angle;
        _updateBBox();
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

    void SceneObject::_updateBBox()
    {
        sf::Transform trans;
        trans.scale(_scale.x, _scale.y);
        trans.rotate(_rotation);
        trans.translate(_pos.x, _pos.y);

        auto bounds = trans.transformRect(_vertices.getBounds());
        _bbox = math::AABBf(bounds.left, bounds.top, bounds.width, bounds.height);
    }

    math::AABBf SceneObject::getLocalBounds() const
    {
        auto bounds = _vertices.getBounds();
        return math::AABBf(bounds.left, bounds.top, bounds.width, bounds.height);
    }
}
