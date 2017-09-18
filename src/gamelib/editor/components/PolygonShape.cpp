#include "gamelib/editor/components/PolygonShape.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    PolygonShape::PolygonShape(size_t size) :
        RenderComponent(name),
        mappingMethod(MapWorld),
        _vertices(sf::TrianglesStrip, size)
    { }

    void PolygonShape::fetch(const math::Polygon<float>& pol)
    {
        _vertices.clear();
        // TODO: reserve

        for (size_t i = 0; i < pol.size(); ++i)
        {
            const math::Point2f& p = pol.getRaw(i);
            _vertices.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Vector2f()));
        }

        _mapTexture(pol);
    }

    void PolygonShape::_mapTexture(const math::Polygon<float>& pol)
    {
        switch (mappingMethod)
        {
            case MapWorld:
            default:
                for (size_t i = 0; i < pol.size(); ++i)
                {
                    const math::Point2f& p = pol.get(i);
                    _vertices[i].texCoords.x = p.x + _texoffset.x;
                    _vertices[i].texCoords.y = p.y + _texoffset.y;
                }
                break;

            case MapLine:
                {
                    float offx = _texoffset.x;
                    unsigned int texh = texture ? texture->getSize().y : 0;

                    for (size_t i = 0; i + 4 <= pol.size(); i += 4)
                    {
                        const math::Point2f p[] = {
                            pol.get(i),
                            pol.get(i + 1),
                            pol.get(i + 2),
                            pol.get(i + 3)
                        };

                        const double d[] = {
                            math::pointDistance(p[0].x, p[0].y, p[2].x, p[2].y),
                            math::pointDistance(p[1].x, p[1].y, p[3].x, p[3].y)
                        };

                        _vertices[i].texCoords = sf::Vector2f(offx, 0);
                        _vertices[i + 1].texCoords = sf::Vector2f(offx, texh);
                        _vertices[i + 2].texCoords = sf::Vector2f(offx + d[0], 0);
                        _vertices[i + 3].texCoords = sf::Vector2f(offx + d[1], texh);

                        offx += std::min(d[0], d[1]);
                    }
                    break;
                }
        }
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

    void PolygonShape::render(sf::RenderTarget& target)
    {
        auto& pos = getEntity()->getTransform().getPosition();
        auto& scale = getEntity()->getTransform().getScale();

        sf::RenderStates states(texture.get());
        states.transform.translate(pos.x, pos.y);
        states.transform.scale(scale.x, scale.y);
        states.transform.rotate(getEntity()->getTransform().getRotation());

        target.draw(_vertices, states);

        // TODO: could be done more efficient, but this is just for testing anyways
        if (flags & render_wireframe && _vertices.getVertexCount() > 2)
        {
            sf::Vertex line[3];
            line[0] = sf::Vertex(_vertices[0].position);
            line[1] = sf::Vertex(_vertices[1].position);
            target.draw(line, 2, sf::LineStrip, states);

            for (size_t i = 2; i < _vertices.getVertexCount(); ++i)
            {
                line[0] = sf::Vertex(_vertices[i - 2].position);
                line[1] = sf::Vertex(_vertices[i].position);
                line[2] = sf::Vertex(_vertices[i - 1].position);
                target.draw(line, 3, sf::LineStrip, states);
            }
        }
    }

    bool PolygonShape::loadFromJson(const Json::Value& node)
    {
        RenderComponent::loadFromJson(node);
        if (node.isMember("texture"))
            texture = ResourceManager::getActive()->get(node["texture"].asString());
        mappingMethod = node.get("mapping", MapWorld).asInt();
        gamelib::loadFromJson(node["texoffset"], _texoffset);

        if (node.isMember("vertices"))
        {
            auto& vertices = node["vertices"];
            _vertices.resize(vertices.size());
            for(Json::ArrayIndex i = 0; i < vertices.size(); ++i)
            {
                gamelib::loadFromJson(vertices[i][0], _vertices[i].position);
                gamelib::loadFromJson(vertices[i][1], _vertices[i].texCoords);
            }
        }
        return true;
    }

    void PolygonShape::writeToJson(Json::Value& node)
    {
        if (texture)
            node["texture"] = texture.getResource()->getPath();
        node["mapping"] = mappingMethod;
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

        RenderComponent::writeToJson(node);
    }
}
