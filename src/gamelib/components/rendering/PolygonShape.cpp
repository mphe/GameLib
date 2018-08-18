#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    PolygonShape::PolygonShape() :
        RenderComponent(name)
    {
        _vertices.setPrimitiveType(sf::TriangleStrip);
    }

    void PolygonShape::fetch(const math::AABBf& rect, MappingMethod mappingMethod)
    {
        _vertices.resize(4);
        _vertices[0].position = sf::Vector2f(0, 0);
        _vertices[1].position = sf::Vector2f(0, rect.h);
        _vertices[2].position = sf::Vector2f(rect.w, 0);
        _vertices[3].position = sf::Vector2f(rect.w, rect.h);
        _texoffset = rect.pos;
        _mapTexture(mappingMethod);
        _updateBBox();
    }

    void PolygonShape::fetch(const math::Polygon<float>& pol, MappingMethod mappingMethod)
    {
        _vertices.clear();
        // TODO: reserve

        for (size_t i = 0; i < pol.size(); ++i)
        {
            const math::Point2f& p = pol.getRaw(i);
            _vertices.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Vector2f()));
        }

        _mapTexture(mappingMethod);
        _updateBBox();
    }

    void PolygonShape::_mapTexture(MappingMethod mappingMethod)
    {
        const sf::Vector2f worldpos(getPosition().x, getPosition().y);

        switch (mappingMethod)
        {
            case MapWorld:
            default:
                for (size_t i = 0; i < size(); ++i)
                    _vertices[i].texCoords = worldpos + _vertices[i].position + sf::Vector2f(_texoffset.x, _texoffset.y);
                break;

            case MapInstance:
                {
                    // auto pos = getLocalBounds().pos;
                    // sf::Vector2f sfpos(pos.x, pos.y);
                    for (size_t i = 0; i < size(); ++i)
                        _vertices[i].texCoords = _vertices[i].position + sf::Vector2f(_texoffset.x, _texoffset.y);
                        // _vertices[i].texCoords = (_vertices[i].position - sfpos) + sf::Vector2f(_texoffset.x, _texoffset.y);
                    break;
                }

            case MapLine:
                {
                    float offx = _texoffset.x;
                    unsigned int texh = texture ? texture->getSize().y : 0;

                    for (size_t i = 0; i + 4 <= size(); i += 4)
                    {
                        const sf::Vector2f p[] = {
                            worldpos + _vertices[i].position,
                            worldpos + _vertices[i + 1].position,
                            worldpos + _vertices[i + 2].position,
                            worldpos + _vertices[i + 3].position,
                        };

                        const double d[] = {
                            math::pointDistance(p[0].x, p[0].y, p[2].x, p[2].y),
                            math::pointDistance(p[1].x, p[1].y, p[3].x, p[3].y)
                        };

                        _vertices[i].texCoords     = sf::Vector2f(offx,        _texoffset.y);
                        _vertices[i + 1].texCoords = sf::Vector2f(offx,        texh + _texoffset.y);
                        _vertices[i + 2].texCoords = sf::Vector2f(offx + d[0], _texoffset.y);
                        _vertices[i + 3].texCoords = sf::Vector2f(offx + d[1], texh + _texoffset.y);

                        offx += std::min(d[0], d[1]);
                    }
                    break;
                }
        }
    }

    void PolygonShape::adaptToTexture()
    {
        auto texw = texture->getSize().x,
             texh = texture->getSize().y;
        fetch(math::AABBf(0, 0, texw, texh), MapInstance);
    }

    void PolygonShape::setTexOffset(float x, float y)
    {
        for (size_t i = 0; i < _vertices.getVertexCount(); ++i)
        {
            _vertices[i].texCoords.x += (x - _texoffset.x);
            _vertices[i].texCoords.y += (y - _texoffset.y);
        }
        _texoffset.fill(x, y);
    }

    void PolygonShape::setTexOffset(const math::Vec2f& vec)
    {
        setTexOffset(vec.x, vec.y);
    }

    const math::Vec2f& PolygonShape::getTexOffset()
    {
        return _texoffset;
    }

    size_t PolygonShape::size() const
    {
        return _vertices.getVertexCount();
    }

    void PolygonShape::render(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = texture.get();
        SceneObject::render(target, states);
    }

    bool PolygonShape::loadFromJson(const Json::Value& node)
    {
        RenderComponent::loadFromJson(node);

        if (node.isMember("texture"))
            texture = ResourceManager::getActive()->get(node["texture"].asString());

        if (node.get("fitTexture", false).asBool())
            adaptToTexture();

        gamelib::loadFromJson(node["texoffset"], _texoffset);

        if (node.isMember("vertices"))
        {
            auto& vertices = node["vertices"];
            _vertices.resize(vertices.size());
            for (Json::ArrayIndex i = 0; i < vertices.size(); ++i)
            {
                gamelib::loadFromJson(vertices[i][0], _vertices[i].position);
                gamelib::loadFromJson(vertices[i][1], _vertices[i].texCoords);
            }
            _updateBBox();
        }

        return true;
    }

    void PolygonShape::writeToJson(Json::Value& node)
    {
        RenderComponent::writeToJson(node);

        if (texture)
            node["texture"] = texture.getResource()->getPath();

        gamelib::writeToJson(node["texoffset"], _texoffset);

        auto& vertices = node["vertices"];
        vertices.resize(_vertices.getVertexCount());

        for (Json::ArrayIndex i = 0; i < _vertices.getVertexCount(); ++i)
        {
            auto& vertex = vertices[i];
            vertex.resize(2);
            gamelib::writeToJson(vertex[0], _vertices[i].position);
            gamelib::writeToJson(vertex[1], _vertices[i].texCoords);
        }
    }
}
