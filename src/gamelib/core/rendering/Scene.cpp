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
        LOG_DEBUG_WARN("Scene destroyed");
    }

    void Scene::update(float elapsed)
    {
        for (auto& i : _cams)
            i.update(elapsed);
    }

    unsigned int Scene::render(sf::RenderTarget& target)
    {
        unsigned int numrendered = 0;

        // This call must be in render(), otherwise new objects won't appear
        // when the update loop is paused.
        _updateQueue();

        if (_cams.empty())
        {
            numrendered = _render(target, target.getView());
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
                numrendered += _render(target, _cams[i].getView());
            }

            target.setView(backup); // reset view
            _currentcam = _default;
        }

        return numrendered;
    }

    unsigned int Scene::_render(sf::RenderTarget& target, const sf::View& view)
    {
        float lastparallax = 1;
        math::AABBf vbox;
        unsigned int numrendered = 0;

        { // Calculate view bounding box
            sf::Vector2f vpos(view.getCenter().x - view.getSize().x / 2,
                    view.getCenter().y - view.getSize().y / 2);

            sf::Transform vtrans;
            vtrans.translate(vpos.x, vpos.y);
            vtrans.rotate(view.getRotation());

            auto camrect = vtrans.transformRect(sf::FloatRect(sf::Vector2f(0, 0), view.getSize()));
            vbox = math::AABBf(camrect.left, camrect.top, camrect.width, camrect.height);
        }

        for (auto& o : _renderQueue)
        {
            // Skip if outside of view
            if (!o->getBBox().intersect(vbox))
            {
                if (o->getBBox().w == 0 || o->getBBox().h == 0)
                    LOG_WARN("SceneObject bounding box has 0 width or height");
                continue;
            }

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
            // else if (!math::almostEquals(lastparallax, parallax))
            else
            {
                auto pos = o->getBBox().getCenter();
                sf::View paraview = target.getView();
                paraview.setCenter(view.getCenter().x + (view.getCenter().x - pos.x) * (parallax - 1),
                                   view.getCenter().y + (view.getCenter().y - pos.y) * (parallax - 1));
                target.setView(paraview);
                lastparallax = parallax;
            }

            o->render(target);
            ++numrendered;
        }

        return numrendered;
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

    Layer::Handle Scene::createLayer(const std::string& name)
    {
        auto hnd = getLayer(name);
        if (hnd.isNull())
        {
            hnd = _layers.acquire();
            _layers[hnd] = Layer(name);
            _layers[hnd]._scene = this;
        }
        else
            LOG_WARN("Layer already exists: ", name, " -> using existing one");

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

    Layer::Handle Scene::getLayer(const std::string& name) const
    {
        for (auto it = _layers.begin(), end = _layers.end(); it != end; ++it)
            if ((*it).getName() == name)
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
            for (auto it = layers.begin(), end = layers.end(); it != end; ++it)
            {
                auto h = createLayer(it.key().asString());

                if (!getLayer(h)->loadFromJson(*it))
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
                i.writeToJson(layers[i.getName()]);
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
            std::stable_sort(_renderQueue.begin(), _renderQueue.end(), [this](SceneObject* a, SceneObject* b) {
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
