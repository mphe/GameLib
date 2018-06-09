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

        _props.registerProperty("sprite", _sprite, +[](SpriteResource::Handle* sprite, const SpriteResource::Handle* val, SpriteComponent* self) {
                self->change(*val);
            }, this);
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
        const auto& rect = _sprite->rect;
        auto tsize = _sprite->tex->getSize();

        int x = rect.x + index * rect.w;
        int y = (rect.y + (int)(x / tsize.x) * rect.h) % tsize.y;
        x = x % tsize.x;

        _vertices[0].texCoords = sf::Vector2f(x, y);
        _vertices[1].texCoords = sf::Vector2f(x, y + rect.h);
        _vertices[2].texCoords = sf::Vector2f(x + rect.w, y);
        _vertices[3].texCoords = sf::Vector2f(x + rect.w, y + rect.h);
    }

    void SpriteComponent::change(const std::string& fname)
    {
        change(getSubsystem<ResourceManager>()->get(fname).as<SpriteResource>());
    }

    void SpriteComponent::change(SpriteResource::Handle sprite)
    {
        _sprite = sprite;
        if (_sprite)
        {
            _ani.ani = _sprite->ani;
            if (_ani.ani.offset < 0)
                _ani.ani.randomize();

            _initShape();
        }
    }

    bool SpriteComponent::loadFromJson(const Json::Value& node)
    {
        if (!node.isMember("sprite"))
        {
            _sprite.reset();
            return false;
        }

        // Allows overriding animation parameters (-> props)
        bool success = RenderComponent::loadFromJson(node);

        if (_ani.ani.offset < 0)
            _ani.ani.randomize();

        _initShape();

        return success;
    }

    void SpriteComponent::writeToJson(Json::Value& node)
    {
        RenderComponent::writeToJson(node);
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
            states.texture = _sprite->tex.get();
            SceneObject::render(target, states);
        }
        else
            SceneObject::render(target, states_);
    }


    void SpriteComponent::_initShape()
    {
        _vertices[0].position = sf::Vector2f(0, 0);
        _vertices[1].position = sf::Vector2f(0, _sprite->rect.h);
        _vertices[2].position = sf::Vector2f(_sprite->rect.w, 0);
        _vertices[3].position = sf::Vector2f(_sprite->rect.w, _sprite->rect.h);

        setIndex(_ani.ani.offset);
    }
}
