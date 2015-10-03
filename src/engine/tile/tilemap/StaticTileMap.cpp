#include "engine/tile/tilemap/StaticTileMap.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "engine/utils/SpriteBatch.hpp"
#include "math/math.hpp"
#include "engine/utils/log.hpp"
#include "engine/tile/TileSet.hpp"
#include "engine/Camera.hpp"

namespace engine
{
    StaticTileMap::StaticTileMap(const TileSet& tileset) :
            _tileset(tileset),
            _size(0, 0),
            _repeat(noRepeat, noRepeat)
    { }


    bool StaticTileMap::loadFromJson(const Json::Value& node)
    {
        destroy();

        //meta data
        setRepeatSize(node.get("virtwidth", noRepeat).asInt(), node.get("virtheight", noRepeat).asInt());
        _size.x = node.get("width", 0).asInt();
        _size.y = node.get("height", 0).asInt();
        _map.resize(_size.x * _size.y, InvalidTile);

        LOG_DEBUG(LOG_DUMP(_repeat.x), ", ", LOG_DUMP(_repeat.y));
        LOG_DEBUG(LOG_DUMP(_size.x), ", ", LOG_DUMP(_size.y));

        // reading tiles
        if (node.isMember("tiles"))
        {
            const auto& tiles = node["tiles"];
            for (Json::ArrayIndex i = 0; i < _map.size() && i < tiles.size(); ++i)
            {
                if (tiles[i].isNumeric())
                {
                    _map[i] = tiles[i].isInt();
                }
                else
                {
                    auto s = tiles[i].asString();
                    if (node.isMember("alias") && node["alias"].isMember(s))
                    {
                        const auto& aliases = node["alias"];
                        if (aliases[s].isString())
                        {
                            s = aliases[s].asString();
                        }
                        else
                        {
                            TileID id = aliases[s].asInt();
                            if (_tileset.hasTile(id))
                                _map[i] = id;
                            else if (id != InvalidTile)
                                LOG_WARN("Tile with ID \"", id, "\" not found -> Skipping");
                            continue;
                        }
                    }

                    if (_tileset.hasTile(s))
                        _map[i] = _tileset.getTile(s).id;
                    else
                        LOG_WARN("Tile \"", s, "\" not found -> Skipping");
                }
            }
        }

        _tiles.reserve(_tileset.size());
        for (size_t i = 0; i < _tileset.size(); ++i)
        {
            _tiles.push_back(_tileset.getTile(i));
        }

        LOG("StaticTileMap loaded");

        return true;
    }

    void StaticTileMap::destroy()
    {
        _map.clear();
        _tiles.clear();
        _size.set(0, 0);
        _repeat.set(noRepeat, noRepeat);
        LOG_WARN("StaticTileMap destroyed");
    }


    TileID StaticTileMap::getTileID(int x, int y) const
    {
        return _get(x / _tileset.getTileSize().x, _tileset.getTileSize().y);
    }

    void StaticTileMap::setRepeatSize(int w, int h)
    {
        _repeat.set(w, h);
    }


    void StaticTileMap::update(float fps)
    {
        if (!_map.empty())
        {
            for (auto& i : _tiles)
            {
                i.update(fps);
            }
        }
    }

    void StaticTileMap::render(sf::RenderTarget& surface, const Camera& cam) const
    {
        if (!_map.empty())
        {
            auto& tsize = _tileset.getTileSize();
            auto camrect = cam.getCamRect();

            geometry::AABB<int> mapsize(
                geometry::Vector2<int>(
                    _repeat.x == infiniteRepeat ? camrect.pos.x : 0,
                    _repeat.y == infiniteRepeat ? camrect.pos.y : 0
                    ),
                geometry::Vector2<int>(
                    _repeat.x == infiniteRepeat ? camrect.size.x : (_repeat.x == noRepeat ? _size.x : _repeat.x) * tsize.x,
                    _repeat.y == infiniteRepeat ? camrect.size.y : (_repeat.y == noRepeat ? _size.y : _repeat.y) * tsize.y
                )
            );

            if (_repeat.x == infiniteRepeat && camrect.pos.x < 0)
            {
                camrect.pos.x -= tsize.x;
                camrect.size.x += tsize.x;
                mapsize.pos.x -= tsize.x;
                mapsize.size.x += tsize.x;
            }
            if (_repeat.y == infiniteRepeat && camrect.pos.y < 0)
            {
                camrect.pos.y -= tsize.y;
                camrect.size.y += tsize.y;
                mapsize.pos.y -= tsize.y;
                mapsize.size.y += tsize.y;
            }

            camrect.crop(mapsize);
            camrect.size += camrect.pos;
            camrect.pos /= tsize;
            camrect.size /= tsize;

            sfutils::SpriteBatch batch(&_tileset.getTexSheet());

            for (int x = camrect.pos.x; x < camrect.size.x; ++x)
            {
                for (int y = camrect.pos.y; y < camrect.size.y; ++y)
                {
                    TileID tile = _get(x, y);

                    if (tile != InvalidTile)
                    {
                        batch.Add(
                            sf::FloatRect(x * tsize.x, y * tsize.y, tsize.x, tsize.y),
                            _tileset.getTexRect(tile, _tiles[tile].getTileData().texdata.offset)
                        );
                    }
                }
            }
            batch.Render(surface);
            _tilesRendered = batch.Size() / 4;
        }
    }


    TileID StaticTileMap::_get(int x, int y) const
    {
        // Applies the repeat-rules to the given coordinate
        // <val> = the coordinate's value
        // <coord> = either x or y
        // no repeat         -> x            -> fixed repeat
        // infinite repeat     -> x % _size.x
        // fixed repeat        -> x < _repeat.x * _size.x ? x % _size.x : x
#       define ADAPT_COORDS(val, coord) \
        (_repeat.coord == infiniteRepeat ? std::abs(val) % _size.coord : _repeat.coord == noRepeat ? (val) : (val) % _size.coord)

        size_t i = ADAPT_COORDS(y, y) * _size.x + ADAPT_COORDS(x, x);
        assert("Out of bounds" && i < _map.size());
        return _map[i];
    }
}
