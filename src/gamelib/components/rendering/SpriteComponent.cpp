#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/core/res/ResourceManager.hpp"

namespace gamelib
{
    SpriteComponent::SpriteComponent() :
        RenderComponent(name),
        _ani(this)
    {
        _vertices.resize(4);
        _vertices.setPrimitiveType(sf::TriangleStrip);

        _props.registerProperty("framepos", _rect.pos);
        _props.registerProperty("framesize", _rect.size);
        _props.registerProperty("texture", _tex);

        // TODO: custom change handlers to automatically update rect coords
    }

    bool SpriteComponent::_init()
    {
        if (!RenderComponent::_init())
            return false;
        if (!_ani._init())
            return false;
        return true;
    }

    void SpriteComponent::_quit()
    {
        RenderComponent::_quit();
        _ani._quit();
    }


    void SpriteComponent::setIndex(int index)
    {
        auto tsize = _tex->getSize();
        int x = (_rect.x + index * _rect.w);
        int y = (_rect.y + x / tsize.x * _rect.h) % tsize.y;
        x = x % tsize.x;

        _vertices[0].texCoords = sf::Vector2f(x, y);
        _vertices[1].texCoords = sf::Vector2f(x, y + _rect.h);
        _vertices[2].texCoords = sf::Vector2f(x + _rect.w, y);
        _vertices[3].texCoords = sf::Vector2f(x + _rect.w, y + _rect.h);
    }

    void SpriteComponent::change(const std::string& fname)
    {
        auto sprite = getSubsystem<ResourceManager>()->get(fname).as<JsonResource>();
        if (sprite)
            loadFromJson(*sprite);

        // Set this after loadFromJson(), to prevent overwriting by nested
        // change() calls
        _sprite = sprite;
    }

    bool SpriteComponent::loadFromJson(const Json::Value& node)
    {
        if (node.isMember("sprite"))
            change(node["sprite"].asString());
        else
            _sprite.reset();

        // Allows overriding sprite parameters (-> props)
        bool success = RenderComponent::loadFromJson(node);

        if (_ani.ani.offset < 0)
            _ani.ani.offset = random() % (int)_ani.ani.length;

        _initShape();

        return success;
    }

    void SpriteComponent::writeToJson(Json::Value& node)
    {
        RenderComponent::writeToJson(node);

        if (_sprite)
            node["sprite"] = _sprite.getResource()->getPath();
    }

    const std::string& SpriteComponent::getSpriteName() const
    {
        return _sprite.getResource()->getPath();
    }

    void SpriteComponent::render(sf::RenderTarget& target, const sf::RenderStates& states_) const
    {
        if (!states_.texture)
        {
            sf::RenderStates states(states_);
            states.texture = _tex.get();
            SceneObject::render(target, states);
        }
        else
            SceneObject::render(target, states_);
    }


    void SpriteComponent::_initShape()
    {
        _vertices[0].position = sf::Vector2f(0, 0);
        _vertices[1].position = sf::Vector2f(0, _rect.h);
        _vertices[2].position = sf::Vector2f(_rect.w, 0);
        _vertices[3].position = sf::Vector2f(_rect.w, _rect.h);

        setIndex(_ani.ani.offset);
    }
}
