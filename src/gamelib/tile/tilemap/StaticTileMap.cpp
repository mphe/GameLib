#include "gamelib/tile/tilemap/StaticTileMap.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "gamelib/utils/SpriteBatch.hpp"
#include "gamelib/utils/log.hpp"
#include "math/math.hpp"

namespace gamelib
{
    constexpr int X = 0;
    constexpr int Y = 1;

    StaticTileMap::StaticTileMap(const SpriteSet& spriteset) :
        _spriteset(spriteset),
        _size{0, 0},
        _tsize{0, 0},
        _virtsize{noRepeat, noRepeat},
        _tilesRendered(0)
    { }



    bool StaticTileMap::loadFromJson(const Json::Value& node)
    {
        destroy();

        //meta data
        _size[X] = node.get("width", 0).asInt();
        _size[Y] = node.get("height", 0).asInt();
        _map.resize(_size[X] * _size[Y], InvalidTile);

        setVirtualSize(node.get("virtwidth", noRepeat).asInt(),
                node.get("virtheight", noRepeat).asInt());

        _tsize[X] = node.get("tilew", 0).asInt();
        _tsize[Y] = node.get("tileh", 0).asInt();

        LOG_DEBUG(LOG_DUMP(_virtsize[X]), ", ", LOG_DUMP(_virtsize[Y]));
        LOG_DEBUG(LOG_DUMP(_size[X]), ", ", LOG_DUMP(_size[Y]));
        LOG_DEBUG(LOG_DUMP(_tsize[X]), ", ", LOG_DUMP(_tsize[Y]));

        TranslationMap trans(_copySprites(node));
        LOG_DEBUG("Tileset contains ", trans.size(), " elements:");
        for (auto& i : trans)
            LOG_DEBUG(i.first, "\t-> ", i.second);

        // Extend the translation map by the defined aliases
        if (node.isMember("alias"))
        {
            const auto& aliases = node["alias"];
            for (auto i = aliases.begin(); i != aliases.end(); ++i)
            {
                if (i->isNumeric())
                {
                    trans[i.key().asString()] = i->asInt();
                    LOG_DEBUG("Found alias ", i.key().asString(), "\t-> ", i->asInt());
                }
                else
                {
                    auto it = trans.find(i->asString());
                    if (it != trans.end())
                    {
                        trans[i.key().asString()] = it->second;
                        LOG_DEBUG("Found alias ", i.key().asString(), "\t-> ", it->second);
                    }
                    else
                    {
                        LOG_WARN("Alias '", i.key().asString(), "' links to unknown value.");
                    }
                }
            }
        }

        if (node.isMember("tiles"))
        {
            const auto& tiles = node["tiles"];
            for (Json::ArrayIndex i = 0; i < _map.size() && i < tiles.size(); ++i)
            {
                TileID id = InvalidTile;
                if (tiles[i].isNumeric())
                {
                    id = tiles[i].isInt();
                }
                else
                {
                    auto it = trans.find(tiles[i].asString());
                    if (it != trans.end())
                        id = it->second;
                }

                if (id < _tiles.size() || id == InvalidTile)
                    _map[i] = id;
                else
                    LOG_WARN("Tile '", id, "' not found -> Skipping");
            }
        }
        else
        {
            LOG_ERROR("No tiles specified.");
            return false;
        }

        LOG("StaticTileMap loaded");

        return true;
    }

    void StaticTileMap::destroy()
    {
        _map.clear();
        _tiles.clear();
        _size[X] = _size[Y] = 0;
        _virtsize[X] = _size[Y] = noRepeat;
        LOG_WARN("StaticTileMap destroyed");
    }


    void StaticTileMap::setVirtualSize(int w, int h)
    {
        _virtsize[X] = w == noRepeat ? _size[X] : w;
        _virtsize[Y] = h == noRepeat ? _size[Y] : h;
    }


    void StaticTileMap::update(float fps)
    {
        if (!_map.empty())
            for (auto& i : _tiles)
                i.update(fps);
    }

    void StaticTileMap::render(sf::RenderTarget& surface, geometry::AABB<int> rect)
    {
        if (!_map.empty())
        {
            geometry::AABB<int> mapsize(
                _virtsize[X] == infiniteRepeat ? rect.pos.x : 0,
                _virtsize[Y] == infiniteRepeat ? rect.pos.y : 0,
                _virtsize[X] == infiniteRepeat ? rect.size.x : _virtsize[X] * _tsize[X],
                _virtsize[Y] == infiniteRepeat ? rect.size.y : _virtsize[Y] * _tsize[Y]
            );

            rect.crop(mapsize);
            rect.size += rect.pos;

            for (size_t i = 0; i < 2; ++i)
            {
                if (rect.size[i] > 0)
                    rect.size[i] += _tsize[i];
                if (rect.pos[i] < 0)
                    rect.pos[i] -= _tsize[i];

                rect.size[i] /= _tsize[i];
                rect.pos[i] /= _tsize[i];
            }

            SpriteBatch batch(&_spriteset.getSpriteSheet());

            const unsigned int texw = _spriteset.getSpriteSheet().getSize().x,
                               texh = _spriteset.getSpriteSheet().getSize().y;

            for (int x = rect.pos.x; x < rect.size.x; ++x)
            {
                for (int y = rect.pos.y; y < rect.size.y; ++y)
                {
                    TileID tile = _get(x, y);

                    if (tile != InvalidTile)
                    {
                        batch.Add(
                            sf::FloatRect(x * _tsize[X], y * _tsize[Y], _tsize[X], _tsize[Y]),
                            _tiles[tile].getRect(texw, texh)
                        );
                    }
                }
            }
            batch.Render(surface);
            _tilesRendered = batch.Size() / 4;
            // LOG_DEBUG(LOG_DUMP(_tilesRendered));
        }
    }


    // Return the tile at x/y (tile coordinates)
    TileID StaticTileMap::_get(int x, int y) const
    {
        x = _adaptCoords(x, X);
        y = _adaptCoords(y, Y);
        if (x < _size[X] && y < _size[Y])
            return _map[y * _size[X] + x];
        return InvalidTile;
    }

    int StaticTileMap::_adaptCoords(int val, int index) const
    {
        if (_virtsize[index] == infiniteRepeat || val < _virtsize[index])
            return std::abs(val) % _size[index];
        return val;
    }

    StaticTileMap::TranslationMap StaticTileMap::_copySprites(const Json::Value& node)
    {
        TranslationMap t; // Stores which SpriteIDs are mapped to which TileIDs
        _tiles.clear();
        if (!node.isMember("sprites") || node["sprites"].size() == 0)
        {
            _tiles.reserve(_spriteset.size());
            for (auto it = _spriteset.begin(); it != _spriteset.end(); ++it)
            {
                t[it->first] = _tiles.size();
                _tiles.push_back(it->second.anidata);
            }
        }
        else
        {
            const auto& sprites = node["sprites"];
            _tiles.reserve(sprites.size());
            for (Json::ArrayIndex i = 0; i < sprites.size(); ++i)
            {
                t[sprites[i].asString()] = i;
                _tiles.push_back(_spriteset.getSpriteData(sprites[i].asString()).anidata);
            }
        }
        return t;
    }
}
