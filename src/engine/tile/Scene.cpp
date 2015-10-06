#include "engine/tile/Scene.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "engine/tile/tilemap/Layer.hpp"
#include "engine/tile/tilemap/TileMap.hpp"
#include "engine/Camera.hpp"
#include "engine/utils/log.hpp"

namespace engine
{
    void Scene::destroy()
    {
        _layers.clear();
        _cams.clear();
        LOG_DEBUG_WARN("Scene destroyed");
    }


    Camera& Scene::addCamera(CameraPtr cam)
    {
        assert("Camera pointer is a null pointer" && cam);
        _cams.push_back(std::move(cam));
        LOG_DEBUG("Added camera to scene");
        return *_cams.back();
    }

    Camera& Scene::getCamera(size_t index) const
    {
        return *_cams[index];
    }


    Layer& Scene::addLayer(LayerPtr layer)
    {
        _layers.push_back(layer ? std::move(layer) : LayerPtr(new Layer()));
        return *_layers.back();
    }

    Layer& Scene::getLayer(size_t index) const
    {
        assert("Index out of bounds" && index < _layers.size());
        return *_layers[index];
    }

    Layer& Scene::getLayer() const
    {
        return *_layers.back();
    }

    size_t Scene::getLayerCount() const
    {
        return _layers.size();
    }


    void Scene::update(float fps)
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

    void Scene::render(sf::RenderTarget& target) const
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
