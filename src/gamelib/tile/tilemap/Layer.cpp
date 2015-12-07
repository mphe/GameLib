#include "gamelib/tile/tilemap/Layer.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "gamelib/utils/log.hpp"
#include "gamelib/tile/tilemap/TileMap.hpp"
#include "gamelib/Camera.hpp"

namespace gamelib
{
    bool Layer::loadFromJson(const Json::Value& node)
    {
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            if (!_mapfactory || !_mapfactory->haskey(it.key().asString()))
            {
                LOG_ERROR("Unknown map id: ", it.key().asString(), " -> Skipping");
                continue;
            }

            TileMapPtr ptr(_mapfactory->create(it.key().asString()));
            if (it->isMember("config") && !ptr->loadFromFile((*it)["config"].asString()))
            {
                LOG_ERROR("Failed to load map -> Skipping");
                continue;
            }

            MapData md;
            md.offset.x = it->get("paraoffsetx", md.offset.x).asInt();
            md.offset.y = it->get("paraoffsety", md.offset.y).asInt();
            md.depth = it->get("paradepth", md.depth).asFloat();
            md.map = std::move(ptr);
            addMap(md);
        }
        return true;
    }

    void Layer::destroy()
    {
        _maps.clear();
        LOG_DEBUG_WARN("Layer destroyed");
    }


    TileMap* Layer::addMap(TileMapPtr map, float depth)
    {
        MapData md;
        md.depth = depth;
        md.map = std::move(map);
        return addMap(md);
    }

    TileMap* Layer::addMap(const MapData& md)
    {
        assert("Map pointer is a null pointer" && md.map);
        _maps.push_back(md);
        _maps.back().map->_layer = this;
        LOG_DEBUG("Added map to layer");
        return _maps.back().map.get();
    }

    void Layer::update(float fps)
    {
        // Update maps from front to back
        for (size_t i = _maps.size(); i > 0; --i)
        {
            _maps[i - 1].map->update(fps);
        }
    }

    void Layer::render(sf::RenderTarget& target, const Camera& cam) const
    {
        sf::View tmp(target.getView()); //view backup
        for(auto& i : _maps)
        {
            Camera paracam(cam);
            paracam.pos = paracam.pos * i.depth + i.offset; //calculate parallax
            target.setView(paracam.getView());
            i.map->render(target, paracam);
        }
        target.setView(tmp);
    }


    void Layer::setMapFactory(const MapFactory* factory)
    {
        _mapfactory = factory;
    }


    TileMap* Layer::getMap(size_t index) const
    {
        assert("Map index out of bounds" && index < _maps.size());
        return _maps[index].map.get();
    }

    TileScene* Layer::getScene() const
    {
        return _scene;
    }

    size_t Layer::size() const
    {
        return _maps.size();
    }


    //MapData
    MapData::MapData() :
            depth(0)
    { }

    MapData::MapData(const MapData& md) :
        depth(md.depth),
        offset(md.offset),
        map(std::move(md.map))
    { }

    MapData& MapData::operator=(const MapData& md)
    {
        map = std::move(md.map);
        depth = md.depth;
        offset = md.offset;
        return *this;
    }
}
