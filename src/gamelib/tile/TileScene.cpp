#include "gamelib/tile/TileScene.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "gamelib/tile/tilemap/Layer.hpp"
#include "gamelib/tile/tilemap/TileMap.hpp"
#include "gamelib/Camera.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    void TileScene::destroy()
    {
        _layers.clear();
        _cams.clear();
        LOG_DEBUG_WARN("Scene destroyed");
    }


    Camera& TileScene::addCamera(CameraPtr cam)
    {
        assert("Camera pointer is a null pointer" && cam);
        _cams.push_back(std::move(cam));
        LOG_DEBUG("Added camera to scene");
        return *_cams.back();
    }

    Camera& TileScene::getCamera(size_t index) const
    {
        return *_cams[index];
    }


    Layer& TileScene::addLayer(LayerPtr layer)
    {
        _layers.push_back(layer ? std::move(layer) : LayerPtr(new Layer()));
        _layers.back()->_scene = this;
        return *_layers.back();
    }

    Layer& TileScene::getLayer(size_t index) const
    {
        assert("Index out of bounds" && index < _layers.size());
        return *_layers[index];
    }

    Layer& TileScene::getLayer() const
    {
        return *_layers.back();
    }

    size_t TileScene::getLayerCount() const
    {
        return _layers.size();
    }


    void TileScene::update(float fps)
    {
        for (size_t i = _layers.size(); i > 0; --i)
        {
            _layers[i - 1]->update(fps);
        }

        for (auto& c : _cams)
        {
            c->update(fps);
        }
    }

    void TileScene::render(sf::RenderTarget& target) const
    {
        for (auto& c : _cams)
        {
            for (auto& l : _layers)
            {
                l->render(target, *c);
            }
            // _cams[i]->render(surface);
        }
    }
}
