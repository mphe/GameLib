#include "gamelib/components/geometry/PixelCollision.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/res/TextureResource.hpp"

namespace gamelib
{
    PixelCollision::PixelCollision() :
        PixelCollision(0, 0, 0, 0, 0)
    { }

    PixelCollision::PixelCollision(float x, float y, float w, float h, unsigned int flags) :
        PixelCollision(math::AABBf(x, y, w, h), flags)
    { }

    PixelCollision::PixelCollision(const math::AABBf& aabb, unsigned int flags_) :
        CollisionComponent(name),
        _mask(sf::Color::Transparent),
        _rect(aabb)
    {
        flags = flags_;
        _supported = movable;
    }

    bool PixelCollision::intersect(const math::Point2f& point) const
    {
        if (_rect.contains(point))
            return _img.getPixel(point.x - _rect.x, point.y - _rect.y).a != 0;
        return false;
    }

    Intersection PixelCollision::intersect(const math::Line2f& line) const
    {
        // TODO: untested

        auto dir = line.d.normalized();
        auto len = line.d.abs();
        auto isec = line.intersect(_rect);

        if (!isec)
            return Intersection();

        auto sign = math::sign(isec.time);
        for (int i = isec.time * len + sign; true; i += sign)
        {
            auto p = isec.p + dir * i;

            if (!_rect.contains(p))
                return Intersection();
            else if (intersect(p))
                return Intersection(p, math::Vec2f(i / len, i / len), getNormal(p));
        }
    }

    Intersection PixelCollision::intersect(const math::AABBf& rect) const
    {
        auto isec = rect.intersect(_rect);
        if (isec)
        {
            // TODO: check only difference

            for (int y = rect.y + rect.h - 1; y >= rect.y; --y)
                for (int x = rect.x; x < rect.x + rect.h; ++x)
                    if (intersect(math::Point2f(x, y)))
                    {
                        return math::Intersection<float>(
                                math::Vec2f((rect.x + rect.w) - x, (rect.y + rect.h) - y),
                                math::Vec2f());
                    }

            // for (int y : { rect.y + rect.h, rect.y })
            //     for (int x = rect.x; x < rect.x + rect.h; ++x)
            //         if (intersect(x, y))
            //             return true;
            //
            // for (int x : { rect.x, rect.x + rect.h })
            //     for (int y = rect.y; y < rect.y + rect.h; ++y)
            //         if (intersect(x, y))
            //             return true;
        }
        return math::Intersection<float>();
    }


    Intersection PixelCollision::sweep(const math::AABBf& rect, const math::Vec2f& vel) const
    {
        math::AABBf rect2(rect.pos + vel, rect.size);
        auto dir = vel.normalized();
        float len = vel.abs();
        int time = ceil(len);

        auto isec = math::Line2f(rect.getCenter().asPoint(), vel, math::Segment).intersect(_rect);
        if (isec)
        {
            for (int i = isec.time * len; i < len; ++i)
            {
                auto p = isec.p + dir * i;
                if (intersect(p))
                {
                    time = i;
                    rect2.center(p);
                    break;
                }
            }
        }

        auto rectisec = intersect(rect2);
        if (rectisec)
        {
            math::Vec2f cp = rect2.pos;
            while (rectisec)
            {
                cp = rect2.pos + (rect2.size - rectisec.delta);
                rect2.pos -= dir;
                --time;
                rectisec = intersect(rect2);
            }

            return math::Intersection<float>(
                    rect2.getCenter().asPoint(),
                    math::Vec2f(time / len, time / len),
                    getNormal(cp.x, cp.y)); // normal seems to bug out sometimes
        }

        return math::Intersection<float>();
    }


    math::Vec2f PixelCollision::getNormal(float x, float y) const
    {
        constexpr int radius = 5;

        math::Vec2f avg;
        for (int w = -radius; w <= radius; ++w)
            for (int h = -radius; h <= radius; ++h)
                if (intersect(math::Point2f(x + w, y + h)))
                    avg -= math::Vec2f(w, h);

        if (avg.x != 0 || avg.y != 0)
            return avg.normalized();
        return avg;
    }

    math::Vec2f PixelCollision::getNormal(const math::Point2f& p) const
    {
        return getNormal(p.x, p.y);
    }


    void PixelCollision::move(const math::Vec2f& rel)
    {
        _rect.pos += rel;
    }

    const math::Point2f& PixelCollision::getPosition() const
    {
        return _rect.pos.asPoint();
    }

    const math::AABBf& PixelCollision::getBBox() const
    {
        return _rect;
    }


    bool PixelCollision::loadFromJson(const Json::Value& node)
    {
        CollisionComponent::loadFromJson(node);

        if (node.isMember("texture"))
            return loadImageFromFile(node["texture"].asString());

        if (node.isMember("mask"))
        {
            auto& mask = node["mask"];
             _mask.r = mask[0].asInt();
             _mask.g = mask[1].asInt();
             _mask.b = mask[2].asInt();
             _mask.a = mask[3].asInt();
        }
        else
            _mask = sf::Color::Transparent;

        return true;
    }

    void PixelCollision::writeToJson(Json::Value& node)
    {
        CollisionComponent::writeToJson(node);

        if (!_texname.empty())
            node["texture"] = _texname;

        auto& mask = node["mask"];
        mask.resize(4);
        mask[0] = _mask.r;
        mask[1] = _mask.g;
        mask[2] = _mask.b;
        mask[3] = _mask.a;
    }


    bool PixelCollision::loadImageFromFile(const std::string& fname)
    {
        auto resmgr = getSubsystem<ResourceManager>();
        if (resmgr)
        {
            auto tex = resmgr->get(fname);
            if (tex)
            {
                loadImageFromTexture(*tex.as<TextureResource>());
                _texname = fname;
                return true;
            }
        }

        // Fall back to normal loading
        if (!_img.loadFromFile(fname))
            return false;

        _img.createMaskFromColor(_mask);
        _texname = fname;
        _rect.size.fill(_img.getSize().x, _img.getSize().y);

        return true;
    }

    void PixelCollision::loadImageFromTexture(const sf::Texture& tex)
    {
        _img = tex.copyToImage();
        _img.createMaskFromColor(_mask);
        _rect.size.fill(_img.getSize().x, _img.getSize().y);
    }

    void PixelCollision::loadImageFromTexture(TextureResource::Handle tex)
    {
        if (tex)
        {
            loadImageFromTexture(*tex);
            _texname = tex.getResource()->getPath();
        }
    }

    const sf::Image& PixelCollision::getImage() const
    {
        return _img;
    }

    sf::Image& PixelCollision::getImage()
    {
        return _img;
    }

    void PixelCollision::setMaskColor(sf::Color mask)
    {
        _mask = mask;
        _img.createMaskFromColor(mask);
    }
}
