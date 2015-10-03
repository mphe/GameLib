#include "tile/tilemap/Layer.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "utils/log.hpp"
#include "tile/tilemap/TileMap.hpp"
#include "Camera.hpp"

namespace engine
{
    void Layer::destroy()
    {
        _maps.clear();
        LOG_WARN("Layer destroyed");
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
        LOG("Added map to layer");
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
