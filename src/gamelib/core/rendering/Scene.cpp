#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/res/JsonResource.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>

namespace gamelib
{
    constexpr const char* Scene::name;

    Scene::Scene() :
        _layerIDcounter(0),
        _currentcam(-1),
        _default(-1),
        _dirty(false)
    {}

    void Scene::destroy()
    {
        _renderQueue.clear();
        _cams.clear();
        _currentcam = -1;
        _default = -1;
        _dirty = false;
        _layers.clear();
        _layerIDcounter = 0;
        LOG_DEBUG_WARN("Scene destroyed");
    }

    void Scene::update(float elapsed)
    {
        for (auto& i : _cams)
            i.update(elapsed);
    }

    void Scene::render(sf::RenderTarget& target)
    {
        _updateQueue();

        if (_cams.empty())
        {
            _render(target, target.getView());
        }
        else
        {
            sf::View backup;  // backup view

            // _default is size_t, that means -1 is integer max, therefore it's bigger
            if (_default >= _cams.size())
                backup = target.getView();
            else
                backup = _cams[_default].getView();

            for (size_t i = 0; i < _cams.size(); ++i)
            {
                _currentcam = i;
                target.setView(_cams[i].getView());
                _render(target, _cams[i].getView());
            }

            target.setView(backup); // reset view
            _currentcam = _default;
        }
    }

    void Scene::_render(sf::RenderTarget& target, const sf::View& view)
    {
        float lastparallax = 1;

        for (auto& o : _renderQueue)
        {
            unsigned int flags = o->flags | this->flags;
            float parallax = o->getParallax() * getParallax();

            if (_layers.isValid(o->getLayer()))
            {
                parallax *= _layers[o->getLayer()].getParallax();
                flags |= _layers[o->getLayer()].flags;
            }

            if (flags & render_invisible)
                continue;

            if (flags & render_hidden && !(flags & render_drawhidden))
                continue;

            if (flags & render_noparallax)
            {
                if (!math::almostEquals(lastparallax, 1.0f))
                {
                    target.setView(view);
                    lastparallax = 1;
                }
            }
            else if (!math::almostEquals(lastparallax, parallax))
            {
                // TODO: object-level parallax needs to be distance(camera, obj) * parallax
                sf::View paraview = target.getView();
                paraview.setCenter(view.getCenter().x * parallax,
                                   view.getCenter().y * parallax);
                target.setView(paraview);
                lastparallax = parallax;
            }

            o->render(target);
        }
    }

    SceneObject* Scene::add(SceneObject* obj)
    {
        assert("SceneObject pointer is a null pointer" && obj);
        _renderQueue.push_back(obj);
        obj->_scene = this;
        _dirty = true;
        LOG_DEBUG("Added SceneObject to scene");
        return obj;
    }

    void Scene::remove(SceneObject* obj)
    {
        assert("SceneObject pointer is a null pointer" && obj);
        auto it = std::find(_renderQueue.begin(), _renderQueue.end(), obj);
        if (it != _renderQueue.end())
        {
            (*it)->_scene = nullptr;
            _renderQueue.erase(it);
            LOG_DEBUG("Removed SceneObject from scene");
        }
    }

    Camera& Scene::addCamera()
    {
        _cams.emplace_back();
        LOG_DEBUG("Added camera to scene");
        return _cams.back();
    }

    Camera& Scene::addCamera(const Camera& cam)
    {
        _cams.push_back(cam);
        LOG_DEBUG("Added camera to scene");
        return _cams.back();
    }

    void Scene::setDefaultCamera(size_t index)
    {
        _default = index;
    }

    Camera* Scene::getCamera(size_t index)
    {
        return (index < _cams.size()) ? &_cams[index] : nullptr;
    }

    const Camera* Scene::getCamera(size_t index) const
    {
        return (index < _cams.size()) ? &_cams[index] : nullptr;
    }

    Camera* Scene::getCurrentCamera()
    {
        return getCamera(_currentcam);
    }

    const Camera* Scene::getCurrentCamera() const
    {
        return getCamera(_currentcam);
    }

    size_t Scene::getCameraCount() const
    {
        return _cams.size();
    }

    Layer::Handle Scene::createLayer()
    {
        auto hnd = _layers.acquire();
        _layers[hnd]._scene = this;
        _layers[hnd]._id = _layerIDcounter++;
        return hnd;
    }

    void Scene::deleteLayer(Layer::Handle handle)
    {
        _layers.destroy(handle);
    }

    const Layer* Scene::getLayer(Layer::Handle handle) const
    {
        return (_layers.isValid(handle)) ? &_layers[handle] : nullptr;
    }

    Layer* Scene::getLayer(Layer::Handle handle)
    {
        return (_layers.isValid(handle)) ? &_layers[handle] : nullptr;
    }

    Layer::Handle Scene::getLayer(size_t layerid) const
    {
        for (auto it = _layers.begin(), end = _layers.end(); it != end; ++it)
            if ((*it).getUniqueID() == layerid)
                return it.handle();
        return Layer::Handle();
    }

    bool Scene::loadFromJson(const Json::Value& node)
    {
        destroy();

        SceneData::loadFromJson(node);

        if (node.isMember("layers"))
        {
            const auto& layers = node["layers"];
            for (const auto& i : layers)
            {
                auto h = createLayer();
                auto layer = getLayer(h);
                if (layer->loadFromJson(i))
                {
                    layer->_id = i.get("id", (Json::UInt)layer->_id).asUInt();
                    if (layer->_id > _layerIDcounter)
                        _layerIDcounter = layer->_id + 1;
                }
                else
                    deleteLayer(h);
            }
        }

        if (node.isMember("cameras"))
        {
            auto& cams = node["cameras"];
            for (const auto& i : cams)
            {
                auto& cam = addCamera();
                const Json::Value* cfg = &i;
                JsonResource::Handle res;

                if (i.isString())
                {
                    auto resmgr = getSubsystem<ResourceManager>();
                    if (resmgr)
                    {
                        res = resmgr->getOnce(i.asString());
                        cfg = &*res;
                    }
                }

                if (!cam.loadFromJson(*cfg))
                    // Don't remove it, because code might rely on the camera count
                    LOG_WARN("Invalid config for camera ", getCameraCount() - 1);
            }
        }

        _default = node.get("defaultcam", 0).asUInt();
        if (_default >= _cams.size())
            _default = _cams.size() - 1;

        _updateQueue();
        return true;
    }

    void Scene::writeToJson(Json::Value& node)
    {
        SceneData::writeToJson(node);

        if (_layers.begin() != _layers.end())
        {
            auto& layers = node["layers"];
            for (auto& i : _layers)
            {
                auto& data = layers.append(Json::Value());
                i.writeToJson(data);
                data["id"] = (Json::UInt)i._id;
            }
        }

        auto& cams = node["cameras"];
        for (auto& i : _cams)
        {
            auto& data = cams.append(Json::Value());
            i.writeToJson(data);
        }

        node["defaultcam"] = (Json::UInt)_default;
    }

    void Scene::_updateQueue()
    {
        if (_dirty)
        {
            LOG_DEBUG("Scene content changed -> sorting");
            std::sort(_renderQueue.begin(), _renderQueue.end(), [this](SceneObject* a, SceneObject* b) {
                    auto la = a->getLayer(),
                         lb = b->getLayer();
                    const int da = _layers.isValid(la) ? _layers[la]._depth : 0,
                              db = _layers.isValid(lb) ? _layers[lb]._depth : 0;
                    return (da != db) ? da > db : a->_depth > b->_depth;
                });
            _dirty = false;
        }
    }
}
