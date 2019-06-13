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

    void AnimationData::setIndex(int index)
    {
        if (index >= 0)
            offset = index % length;
        else
            setIndex(length + index);
    }

    void AnimationData::randomize()
    {
        offset = rand() % length;
    }



    sf::IntRect SpriteData::getRect(int texw, int texh) const
    {
        return getSpriteRect(anidata.offset, rect, texw, texh);
    }


    sf::IntRect getSpriteRect(int offset, const sf::IntRect& rect, int texw, int texh)
    {
        auto newrect = getSpriteRect(offset, math::AABBi(rect.left, rect.top, rect.width, rect.height), texw, texh);
        return sf::IntRect(newrect.x, newrect.y, newrect.w, newrect.h);
    }

    math::AABBi getSpriteRect(int offset, const math::AABBi& rect, int texw, int texh)
    {
        int x = rect.x + (int)offset * rect.w;
        return math::AABBi(x % texw, (rect.y + x / texw * rect.h) % texh, rect.w, rect.h);
    }
}
