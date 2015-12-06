#include "engine/Sprite.hpp"

namespace engine
{
    AnimationData::AnimationData() :
        startx(0),
        starty(0),
        length(0),
        speed(0),
        offset(0)
    { }

    void AnimatedSprite::update(float fps)
    {
        anidata.offset = std::fmod(anidata.offset + anidata.speed / fps, anidata.length);
        sf::IntRect rect = sfmlsprite.getTextureRect();
        int x = anidata.startx + (int)anidata.offset * rect.width;
        rect.left = x % sfmlsprite.getTexture()->getSize().x;
        rect.top = anidata.starty + x / sfmlsprite.getTexture()->getSize().x * rect.height,
        sfmlsprite.setTextureRect(rect);
    }
}
