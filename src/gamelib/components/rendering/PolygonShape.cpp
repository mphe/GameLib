#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    PolygonShape::PolygonShape() :
        RenderComponent(name),
        _texscale(1, 1),
        _mapping(MapInstance)
    {
        _vertices.setPrimitiveType(sf::TriangleStrip);

        _props.registerProperty("texture", texture);
        _props.registerProperty("texoffset", _texoffset, PROP_METHOD(math::Vec2f, setTexOffset), this);
        _props.registerProperty("texscale", _texscale, PROP_METHOD(math::Vec2f, setTexScale), this);
        _props.registerProperty("mapping", _mapping, PROP_METHOD(MappingMethod, setMappingMethod), this, 0, NumMappingMethods, mapping_strings);
    }

    void PolygonShape::fetch(const math::AABBf& rect)
    {
        _vertices.resize(4);
        _vertices[0].position = sf::Vector2f(0, 0);
        _vertices[1].position = sf::Vector2f(0, rect.h);
        _vertices[2].position = sf::Vector2f(rect.w, 0);
        _vertices[3].position = sf::Vector2f(rect.w, rect.h);
        _mapTexture();
        _updateBBox();
    }

    void PolygonShape::fetch(const math::BasePolygon<float>& pol, bool raw)
    {
        _vertices.resize(pol.size());

        for (size_t i = 0; i < pol.size(); ++i)
            _vertices[i] = sf::Vertex(convert((raw ? pol.getRaw(i) : pol.get(i)).asVector()));

        _mapTexture();
        _updateBBox();
    }


    void PolygonShape::adaptToTexture()
    {
        _mapping = MapInstance;
        fetch(math::AABBf(0, 0, texture->getSize().x, texture->getSize().y));
    }


    void PolygonShape::setMappingMethod(MappingMethod m)
    {
        _mapping = m;
        _mapTexture();
    }

    MappingMethod PolygonShape::getMappingMethod() const
    {
        return _mapping;
    }

    void PolygonShape::setTexOffset(const math::Vec2f& vec)
    {
        auto diff = convert(vec - _texoffset);
        for (size_t i = 0; i < size(); ++i)
            _vertices[i].texCoords += diff;
        _texoffset = vec;
    }

    const math::Vec2f& PolygonShape::getTexOffset() const
    {
        return _texoffset;
    }


    void PolygonShape::setTexScale(const math::Vec2f& vec)
    {
        _texscale = vec;
        _mapTexture();
    }

    const math::Vec2f& PolygonShape::getTexScale() const
    {
        return _texscale;
    }


    void PolygonShape::render(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = texture.get();
        SceneObject::render(target, states);
    }


    bool PolygonShape::loadFromJson(const Json::Value& node)
    {
        RenderComponent::loadFromJson(node);

        if (node.isMember("vertices"))
        {
            auto& vertices = node["vertices"];
            _vertices.resize(vertices.size());
            for (Json::ArrayIndex i = 0; i < vertices.size(); ++i)
                gamelib::loadFromJson(vertices[i], _vertices[i].position, true);
            _mapTexture();
            _updateBBox();
        }

        return true;
    }

    void PolygonShape::writeToJson(Json::Value& node) const
    {
        RenderComponent::writeToJson(node);

        auto& vertices = node["vertices"];
        vertices.resize(size());

        for (Json::ArrayIndex i = 0; i < size(); ++i)
            gamelib::writeToJson(vertices[i], _vertices[i].position);
    }


    void PolygonShape::_mapTexture()
    {
        _mapTexture(_mapping);
    }

    void PolygonShape::_mapTexture(MappingMethod mappingMethod)
    {
        switch (mappingMethod)
        {
            default:
            case MapWorld:
                for (size_t i = 0; i < size(); ++i)
                    _vertices[i].texCoords = convert(getPosition().asVector()) + _vertices[i].position;
                break;

            case MapInstance:
                for (size_t i = 0; i < size(); ++i)
                    _vertices[i].texCoords = _vertices[i].position;
                break;

            case MapLine:
                {
                    float offx = 0;
                    unsigned int texh = texture ? texture->getSize().y : 0;
                    math::Vec2f p[4];

                    for (size_t i = 0; i + 4 <= size(); i += 4)
                    {
                        for (int ii = 0; ii < 4; ++ii)
                            p[ii] = convert(_vertices[i + ii].position);

                        double d1 = (p[2] - p[0]).abs(),
                               d2 = (p[3] - p[1]).abs();

                        _vertices[i    ].texCoords = sf::Vector2f(offx,      0);
                        _vertices[i + 1].texCoords = sf::Vector2f(offx,      texh);
                        _vertices[i + 2].texCoords = sf::Vector2f(offx + d1, 0);
                        _vertices[i + 3].texCoords = sf::Vector2f(offx + d2, texh);

                        offx += std::min(d1, d2);
                    }
                    break;
                }
        }

        auto off = _texoffset;
        _texoffset.fill(0);
        setTexOffset(off);

        // apply scale
        for (size_t i = 0; i < size(); ++i)
            convert(_vertices[i].texCoords) /= _texscale;
    }

    void PolygonShape::_onChanged(const sf::Transform& old)
    {
        RenderComponent::_onChanged(old);
        _mapTexture();
    }
}
