#include "gamelib/core/sprite/SpriteData.hpp"
#include <cmath>

namespace gamelib
{
    AnimationData::AnimationData() :
        length(1),
        speed(0),
        offset(0)
    { }

    void AnimationData::update(float elapsed)
    {
        if (length > 1 && speed != 0)
        {
            offset = std::fmod(offset + speed * elapsed, length);
            if (offset < 0)
                offset = length - offset;
        }
    }


    sf::IntRect SpriteData::getRect(int texw, int texh) const
    {
        return getSpriteRect(anidata.offset, rect, texw, texh);
    }


    sf::IntRect getSpriteRect(int offset, const sf::IntRect& rect, int texw, int texh)
    {
        int x = rect.left + (int)offset * rect.width;
        return sf::IntRect(x % texw, (rect.top + x / texw * rect.height) % texh, rect.width, rect.height);
    }
}
