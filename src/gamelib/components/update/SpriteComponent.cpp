#include "gamelib/components/update/SpriteComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/json.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/core/res/JsonResource.hpp"

namespace gamelib
{
    SpriteComponent::SpriteComponent() :
        UpdateComponent(name),
        _shape(nullptr)
    { }

    void SpriteComponent::update(float elapsed)
    {
        int old = ani.offset;
        ani.update(elapsed);

        if (old != (int)ani.offset)
            _updateShape();
    }

    void SpriteComponent::change(const std::string& fname)
    {
        loadFromJson(*getSubsystem<ResourceManager>()->get(fname).as<JsonResource>());
        _spritename = fname;
    }

    bool SpriteComponent::loadFromJson(const Json::Value& node)
    {
        UpdateComponent::loadFromJson(node);

        _spritename.clear();

        if (node.isMember("sprite"))
        {
            change(node["sprite"].asString());
        }
        else
        {
            ani.length = node.get("length", 1).asFloat();
            ani.speed = node.get("speed", 0).asFloat();
            ani.offset = node.get("index", 0).asFloat();
            if (ani.offset < 0)
                ani.offset = random() % (int)ani.length;


            math::Vec2f pos, size;
            gamelib::loadFromJson(node["pos"], pos);
            gamelib::loadFromJson(node["size"], size);
            ani.rect = sf::IntRect(pos.x, pos.y, size.x, size.y);

            if (node.isMember("texture"))
                _tex = getSubsystem<ResourceManager>()->get(node["texture"].asString());

            _initShape();
        }

        return true;
    }

    const std::string& SpriteComponent::getSpriteName() const
    {
        return _spritename;
    }

    void SpriteComponent::writeToJson(Json::Value& node)
    {
        UpdateComponent::writeToJson(node);

        if (!_spritename.empty())
            node["sprite"] = _spritename;
        else
        {
            node["length"] = ani.length;
            node["speed"] = ani.speed;
            node["index"] = ani.offset;
            gamelib::writeToJson(node["pos"], math::Vec2f(ani.rect.left, ani.rect.top));
            gamelib::writeToJson(node["size"], math::Vec2f(ani.rect.width, ani.rect.height));

            if (_tex)
                node["texture"] = _tex.getResource()->getPath();
        }
    }

    void SpriteComponent::_updateShape()
    {
        if (_shape && _tex)
        {
            auto rect = ani.getRect(_tex->getSize().x, _tex->getSize().y);
            _shape->setTexOffset(rect.left, rect.top);
        }
    }

    void SpriteComponent::_initShape()
    {
        if (_shape && _tex)
            _shape->texture = _tex;

        if (_shape)
            _shape->fetch(math::AABBf(ani.rect.left, ani.rect.top, ani.rect.width, ani.rect.height), MapInstance);

        _updateShape();
    }

    void SpriteComponent::_refresh()
    {
        auto old = _shape;
        _shape = getEntity()->findByName<PolygonShape>();

        if (_shape != old)
            _initShape();
    }
}
