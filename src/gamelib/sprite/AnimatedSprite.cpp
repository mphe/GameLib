#include "gamelib/sprite/AnimatedSprite.hpp"

namespace gamelib
{
    AnimatedSprite::AnimatedSprite(const sf::Texture& tex) :
        sf::Sprite(tex)
    { }

    AnimatedSprite::AnimatedSprite(const AnimationData& anidata) :
        _anidata(anidata)
    { }

    AnimatedSprite::AnimatedSprite(const SpriteData& spritedata) :
        _anidata(spritedata.anidata)
    {
        setOrigin(spritedata.origin);
    }

    AnimatedSprite::AnimatedSprite(const sf::Texture& tex, const AnimationData& anidata) :
        sf::Sprite(tex),
        _anidata(anidata)
    { }

    AnimatedSprite::AnimatedSprite(const sf::Texture& tex, const SpriteData& spritedata) :
        sf::Sprite(tex),
        _anidata(spritedata.anidata)
    {
        setOrigin(spritedata.origin);
    }


    void AnimatedSprite::update(float elapsed)
    {
        _anidata.update(elapsed);
        setTextureRect(_anidata.getRect(getTexture()->getSize().x, getTexture()->getSize().y));
    }

    void AnimatedSprite::render(sf::RenderTarget& target)
    {
        target.draw(*this);
    }

    void AnimatedSprite::change(const AnimationData& anidata)
    {
        _anidata = anidata;
        setTextureRect(_anidata.getRect(getTexture()->getSize().x, getTexture()->getSize().y));
    }

    void AnimatedSprite::change(const SpriteData& spritedata)
    {
        change(spritedata.anidata);
        setOrigin(spritedata.origin);
    }
}
