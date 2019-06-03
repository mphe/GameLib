#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/properties/PropResource.hpp"
#include <SFML/Graphics/Vertex.hpp>

namespace gamelib
{
    SpriteComponent::SpriteComponent() :
        _ani(this)
    {
        registerResourceProperty(_props, "sprite", _sprite, PROP_METHOD(_sprite, change), this);

        // Don't register these properties, because it can't be guaranteed after serializaion
        // that these are loaded _after_ sprite is loaded and therefore overwrite the default
        // values.
        // That means these properties are unreliable and need extra code to work properly.
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

        _system->createNodeMesh(_handle, 4, sf::TriangleStrip);
        _system->setNodeMeshSize(_handle, 0);   // Don't render anything when no sprite is set
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

        if (!_sprite)
        {
            _system->setNodeMeshSize(_handle, 0);
            return;
        }

        _ani.ani = _sprite->ani;
        if (_ani.ani.offset < 0)
            _ani.ani.randomize();

        sf::Vector2f vertices[] = {
            sf::Vector2f(0, _sprite->rect.h),
            sf::Vector2f(_sprite->rect.w, 0),
            sf::Vector2f(_sprite->rect.w, _sprite->rect.h),
        };

        _system->setNodeOptions(_handle, nullptr, nullptr, nullptr, &_sprite->tex);
        _system->updateNodeMesh(_handle, 3, 1, vertices);
        _updateUV();
        setOrigin(sprite->origin);
    }

    SpriteResource::Handle SpriteComponent::getSprite() const
    {
        return _sprite;
    }

    const std::string& SpriteComponent::getSpriteName() const
    {
        return _sprite.getResource()->getPath();
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

        sf::Vector2f uv[] = {
            sf::Vector2f(x + magic, y + magic),
            sf::Vector2f(x + magic, y + rect.h - magic),
            sf::Vector2f(x + rect.w - magic, y + magic),
            sf::Vector2f(x + rect.w - magic, y + rect.h - magic),
        };
        _system->updateNodeMesh(_handle, 4, 0, nullptr, uv);
    }

    auto SpriteComponent::getAnimation() const -> const AnimationComponent&
    {
        return _ani;
    }

    auto SpriteComponent::getTexture() const -> TextureResource::Handle
    {
        return _sprite->tex;
    }
}
