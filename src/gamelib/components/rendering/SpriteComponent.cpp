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

        auto spritesetter = +[](SpriteResource::Handle*, const SpriteResource::Handle* val, SpriteComponent* self)
        {
            self->change(*val);
        };

        _props.registerProperty("sprite", _sprite, spritesetter, this);

        // Don't register these properties, because it can't be guaranteed after serializaion
        // that these are loaded _after_ sprite is loaded and therefore overwrite the default
        // values.
        // That means these properties are unreliable need extra code to work properly.
        // TODO: Fix this
        // _props.registerProperty("offset", _ani.ani.offset, 0, _ani.ani.length);
        // _props.registerProperty("speed", _ani.ani.speed);

        // props->registerProperty("length", ani.length);
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
        _ani.ani.setIndex(index);
        _updateUV();
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

            setOrigin(sprite->origin);
            _initShape();
        }
    }

    bool SpriteComponent::loadFromJson(const Json::Value& node)
    {
        // if (!node.isMember("sprite"))
        // {
        //     _sprite.reset();
        //     return false;
        // }

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

    SpriteResource::Handle SpriteComponent::getSprite() const
    {
        return _sprite;
    }

    const std::string& SpriteComponent::getSpriteName() const
    {
        return _sprite.getResource()->getPath();
    }

    void SpriteComponent::render(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (_sprite)
        {
            states.texture = _sprite->tex.get();
            SceneObject::render(target, states);
        }
    }


    void SpriteComponent::_initShape()
    {
        if (!_sprite)
            return;

        _vertices[0].position = sf::Vector2f(0, 0);
        _vertices[1].position = sf::Vector2f(0, _sprite->rect.h);
        _vertices[2].position = sf::Vector2f(_sprite->rect.w, 0);
        _vertices[3].position = sf::Vector2f(_sprite->rect.w, _sprite->rect.h);
        _updateBBox();

        setIndex(_ani.ani.offset);
    }

    void SpriteComponent::_updateUV()
    {
        if (!_sprite)
            return;

        // Adding this to the tex coords will prevent the 1px border glitch (hopefully)
        // https://gamedev.stackexchange.com/a/75244
        // https://stackoverflow.com/questions/19611745/opengl-black-lines-in-between-tiles
        constexpr float magic = 0.375;

        const auto& rect = _sprite->rect;
        auto tsize = _sprite->tex->getSize();

        int x = rect.x + (int)_ani.ani.offset * rect.w;
        int y = (rect.y + (int)(x / tsize.x) * rect.h) % tsize.y;
        x = x % tsize.x;

        _vertices[0].texCoords = sf::Vector2f(x + magic, y + magic);
        _vertices[1].texCoords = sf::Vector2f(x + magic, y + rect.h - magic);
        _vertices[2].texCoords = sf::Vector2f(x + rect.w - magic, y + magic);
        _vertices[3].texCoords = sf::Vector2f(x + rect.w - magic, y + rect.h - magic);
    }
}
